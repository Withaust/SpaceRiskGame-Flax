#include "Networking.h"
#include <Game/Shared/Components/PlayerOwned.h>

UIMPL_SINGLETON(Networking)

Networking::Networking(const SpawnParams& params)
    : ISystem(params)
{
    _tickUpdate = true;
}

void Networking::ForceAddReplicated(INetworkedObject* obj)
{
    _spawnList.Add(obj);
}

void Networking::OnNetworkStateChanged()
{
    switch (NetworkManager::State)
    {
    case NetworkConnectionState::Connected:
    {
#if !BUILD_RELEASE
        if (Engine::MainWindow)
        {
            Engine::MainWindow->SetTitle(String::Format(TEXT("{} - {}"), NetworkManager::IsClient() ? TEXT("Client") : TEXT("Host"), NetworkManager::LocalClientId));
        }
#endif
        Core::Instance->OnConnected();
        break;
    }
    case NetworkConnectionState::Offline:
    case NetworkConnectionState::Disconnected:
    {
        Core::Instance->OnDisconnected();
        break;
    }
    }
}

void Networking::OnNetworkClientConnected(NetworkClient* client)
{
    Core::Instance->OnPlayerConnected(client);
}

void Networking::OnNetworkClientDisconnected(NetworkClient* client)
{
    Core::Instance->OnPlayerDisconnected(client);
}

void Networking::OnSynced(NetworkClient* client)
{
    Array<ScriptingObjectReference<INetworkedObject>> deleted;
    for (const auto& spawn : _spawnList)
    {
        if (spawn.Item)
        {
            SyncEvent newEvent;
            newEvent.client = client;
            newEvent.object = spawn.Item;
            _syncEvents.Enqueue(newEvent);
        }
        else
        {
            deleted.Add(spawn.Item);
        }
    }
    for (const auto& invalid : deleted)
    {
        _spawnList.Remove(invalid);
    }
}

void Networking::BindEvents()
{
    NetworkManager::StateChanged.Bind<Networking, &Networking::OnNetworkStateChanged>(this);
    NetworkManager::ClientConnected.Bind<Networking, &Networking::OnNetworkClientConnected>(this);
    NetworkManager::ClientDisconnected.Bind<Networking, &Networking::OnNetworkClientDisconnected>(this);
}

void Networking::UnbindEvents()
{
    NetworkManager::StateChanged.Unbind<Networking, &Networking::OnNetworkStateChanged>(this);
    NetworkManager::ClientConnected.Unbind<Networking, &Networking::OnNetworkClientConnected>(this);
    NetworkManager::ClientDisconnected.Unbind<Networking, &Networking::OnNetworkClientDisconnected>(this);
}

void Networking::OnInitialize()
{
#if !BUILD_RELEASE
    NetworkReplicator::EnableLog = true;
#endif
    _stream = New<NetworkStream>();
}

void Networking::OnDeinitialize()
{
    UnbindEvents();
    Delete(_stream);
    UDEINIT_SINGLETON();
}

void Networking::OnUpdate()
{
    if (NetworkManager::IsClient())
    {
        SyncInfo::Instance->RequestSpawnSync();
        return;
    }
    else if (_syncEvents.Count() == 0)
    {
        return;
    }

    SyncEvent sync = _syncEvents.Dequeue();

    if (!NetworkManager::Clients.Contains(sync.client) || NetworkReplicator::GetObjectOwnerClientId(sync.object) == sync.client->ClientId)
    {
        return;
    }

    if (!sync.object)
    {
        return;
    }

    NetworkStream* stream = Networking::Instance->_stream;
    stream->Initialize();

    if (INetworkSerializable* networked = ToInterface<INetworkSerializable>(sync.object))
    {
        networked->Serialize(stream);
    }
    else
    {
        NetworkReplicator::InvokeSerializer(sync.object->GetTypeHandle(), sync.object, stream, true);
    }

    if (!EngineHelper::Compress(stream->GetBuffer(), stream->GetLength()))
    {
        UCRIT(true, "EngineHelper::Compress failed to compress replication data for {0}.", sync.object->GetType().Fullname.ToString());
        return;
    }

    NetworkRpcParams params;
    uint32 ids[1] = { sync.client->ClientId };
    params.TargetIds = ToSpan(ids, ARRAY_COUNT(ids));
    sync.object->SendData(EngineHelper::GetCompressBuffer(), stream->GetLength(), params);
}

void Networking::StartGame()
{
    const auto instance = LaunchArgs::Instance;
    NetworkSettings* settings = NetworkSettings::Get();
    settings->NetworkFPS = -1.0f;
    settings->Address = instance->Hostname;
    settings->Port = 40808;
    settings->MaxClients = 6;
    settings->Apply();

    if (instance->IsHost)
    {
        NetworkManager::StartHost();
    }
    else
    {
        NetworkManager::StartClient();
    }
}

ScriptingObjectReference<Entity> Networking::SpawnPrefab(Prefab* prefab, Actor* parent, uint32 ownerId, const Vector3& position, const Quaternion& rotation)
{
    Actor* actor = PrefabManager::SpawnPrefab(prefab, parent);
    ScriptingObjectReference<Entity> target = Cast<Entity>(actor);
    if (!target)
    {
        Logger::Instance->Error(TEXT("Tried to spawn prefab ") + actor->GetName() + TEXT(" but it was not an entity!"));
        return {};
    }

    NetworkObjectRole role = NetworkManager::LocalClientId == ownerId ? NetworkObjectRole::OwnedAuthoritative : NetworkObjectRole::ReplicatedSimulated;

    NetworkReplicator::SpawnObject(target);
    for (int i = 0; i < target->Scripts.Count(); ++i)
    {
        IComponent* comp = Cast<IComponent>(target->Scripts[i]);
        if (comp)
        {
            if (comp->Type != ObjNetType::None)
            {
                NetworkReplicator::SpawnObject(target->Scripts[i]);
            }
            if (comp->Type == ObjNetType::Replicated)
            {
                _spawnList.Add(comp);
            }
            if (PlayerOwned* owned = Cast<PlayerOwned>(comp))
            {
                owned->SetServerIdRemote(target->GetID());
            }
        }
    }

    NetworkReplicator::SetObjectOwnership(target, ownerId, role, true);

    Level::SpawnActor(target, parent);
#if !BUILD_RELEASE
    if (target->Type != EntNetType::None)
    {
        Logger::Instance->Error(TEXT("Tried to spawn prefab ") + prefab->GetPath() + TEXT(" but its root entity was not marked as LevelReplication = None!"));
    }
#endif
    target->SetTransform(Transform(position, rotation));

    // Force executing on a spawned prefab even not on networked event
    for (int i = 0; i < target->Scripts.Count(); ++i)
    {
        INetworkObject* netObj = ScriptingObject::ToInterface<INetworkObject>(target->Scripts[i]);
        if (netObj)
        {
            netObj->OnNetworkSpawn();
        }
    }

    return target;
}

void Networking::DespawnPrefab(ScriptingObjectReference<Entity> target)
{
    // Force executing on a spawned prefab even not on networked event
    for (int i = 0; i < target->Scripts.Count(); ++i)
    {
        INetworkObject* netObj = ScriptingObject::ToInterface<INetworkObject>(target->Scripts[i]);
        if (netObj)
        {
            netObj->OnNetworkDespawn();
        }
    }

    for (int i = 0; i < target->Scripts.Count(); ++i)
    {
        IComponent* comp = Cast<IComponent>(target->Scripts[i]);
        if (comp)
        {
            if (comp->Type != ObjNetType::None)
            {
                NetworkReplicator::DespawnObject(target->Scripts[i]);
            }
            if (comp->Type == ObjNetType::Replicated)
            {
                _spawnList.Remove(comp);
            }
        }
    }

    NetworkReplicator::DespawnObject(target);
}

void Networking::StartReplicating(Entity* target)
{
    NetworkReplicator::AddObject(target);
    for (int i = 0; i < target->Scripts.Count(); ++i)
    {
        IComponent* comp = Cast<IComponent>(target->Scripts[i]);
        if (comp)
        {
            if (comp->Type != ObjNetType::None)
            {
                NetworkReplicator::AddObject(target->Scripts[i]);
            }
            if (comp->Type == ObjNetType::Replicated)
            {
                _spawnList.Add(comp);
            }
        }
    }
    if (NetworkManager::LocalClientId == NetworkManager::ServerClientId)
    {
        NetworkReplicator::SetObjectOwnership(target, NetworkManager::ServerClientId, NetworkObjectRole::OwnedAuthoritative, true);
    }
    else
    {
        NetworkReplicator::SetObjectOwnership(target, NetworkManager::ServerClientId, NetworkObjectRole::ReplicatedSimulated, true);
    }

    INetworkObject* netObj;
    for (int i = 0; i < target->Scripts.Count(); ++i)
    {
        netObj = ScriptingObject::ToInterface<INetworkObject>(target->Scripts[i]);
        if (netObj)
        {
            netObj->OnNetworkSpawn();
        }
    }
}

void Networking::StopReplicating(Entity* target)
{
    for (int i = 0; i < target->Scripts.Count(); ++i)
    {
        INetworkObject* netObj = ScriptingObject::ToInterface<INetworkObject>(target->Scripts[i]);
        if (netObj)
        {
            netObj->OnNetworkDespawn();
        }
    }
    for (int i = 0; i < target->Scripts.Count(); ++i)
    {
        IComponent* comp = Cast<IComponent>(target->Scripts[i]);
        if (comp)
        {
            if (comp->Type != ObjNetType::None)
            {
                NetworkReplicator::RemoveObject(target->Scripts[i]);
            }
            if (comp->Type == ObjNetType::Replicated)
            {
                _spawnList.Remove(comp);
            }
        }
    }
    NetworkReplicator::RemoveObject(target);
}

void Networking::DespawnReplicating(Entity* target)
{
    for (int i = 0; i < target->Scripts.Count(); ++i)
    {
        INetworkObject* netObj = ScriptingObject::ToInterface<INetworkObject>(target->Scripts[i]);
        if (netObj)
        {
            netObj->OnNetworkDespawn();
        }
    }
    target->Despawning = true;
    for (int i = 0; i < target->Scripts.Count(); ++i)
    {
        IComponent* comp = Cast<IComponent>(target->Scripts[i]);
        if (comp)
        {
            if (comp->Type != ObjNetType::None)
            {
                NetworkReplicator::DespawnObject(target->Scripts[i]);
            }
            if (comp->Type == ObjNetType::Replicated)
            {
                _spawnList.Remove(comp);
            }
        }
    }
    NetworkReplicator::DespawnObject(target);
}

Guid Networking::SerializeEntity(Entity* target)
{
    if (PlayerOwned* owned = target->GetComponent<PlayerOwned>())
    {
        return owned->GetServerIdLocal();
    }
    return target->GetID();
}

Entity* Networking::DeserializeEntity(Guid id, NetworkRpcParams rpcParams)
{
    return Cast<Entity>(NetworkReplicator::ResolveForeignObject(id));
}

Span<uint32> Networking::GetClientIdsExcept(uint32 exception)
{
    Array<uint32> clients;
    for (const auto& client : NetworkManager::Clients)
    {
        if (client->ClientId != exception)
        {
            clients.Add(client->ClientId);
        }
    }
    return ToSpan(clients.Get(), clients.Count());
}