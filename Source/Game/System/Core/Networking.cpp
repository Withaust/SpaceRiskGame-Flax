#include "Networking.h"
#include <Game/Shared/Components/PlayerOwned.h>

UIMPL_SINGLETON(Networking)

Networking::Networking(const SpawnParams& params)
    : ISystem(params)
{
    _tickUpdate = true;
}

void Networking::AddReplicatedSystem(ScriptingObjectReference<ScriptingObject> obj)
{
    if (NetworkManager::IsHost())
    {
        _spawnList[ReplicationPriority::High].Add(obj);
    }
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

void Networking::OnNetworkClientLoaded(NetworkClient* client)
{
    Core::Instance->OnPlayerLoaded(client);
}

void Networking::OnNetworkClientDisconnected(NetworkClient* client)
{
    Core::Instance->OnPlayerDisconnected(client);
}

void Networking::OnSynced(NetworkClient* client)
{
    Dictionary<ReplicationPriority, Array<ScriptingObjectReference<ScriptingObject>>> deleted;

    for (byte i = static_cast<byte>(ReplicationPriority::High); i < static_cast<byte>(ReplicationPriority::Max); i++)
    {
        ReplicationPriority priority = static_cast<ReplicationPriority>(i);
        const auto& targetPriority = _spawnList[priority];
        uint32 counter = 0;

        for (const auto& entry : targetPriority)
        {
            if (entry.Item)
            {
                SyncEvent newEvent;
                newEvent.client = client;
                newEvent.object = entry.Item;
                _syncEvents.Enqueue(newEvent);
                counter++;
            }
            else
            {
                deleted[priority].Add(entry.Item);
            }
        }

        UINFO("Replicating {0} {1} priority entities.", counter, ScriptingEnum::ToString(priority));
    }

    for (const auto& priority : deleted)
    {
        for (const auto& entry : priority.Value)
        {
            _spawnList[priority.Key].Remove(entry);
        }
    }

    OnNetworkClientLoaded(client);
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

    _spawnList[ReplicationPriority::Low] = {};
    _spawnList[ReplicationPriority::Medium] = {};
    _spawnList[ReplicationPriority::High] = {};
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
        return;
    }
    else if (_syncEvents.Count() == 0)
    {
        return;
    }

    while (_syncEvents.Count() != 0)
    {
        SyncEvent sync = _syncEvents.Dequeue();

        if (!NetworkManager::Clients.Contains(sync.client))
        {
            return;
        }

        if (!sync.object || NetworkReplicator::GetObjectOwnerClientId(sync.object) == sync.client->ClientId)
        {
            return;
        }

        NetworkStream* stream = Networking::Instance->_stream;

        ScriptingObjectReference<Entity> entity = Cast<Entity>(sync.object);

        // Entity has custom replication code that groups up and compresses all of the networked components inside
        if (entity)
        {
            Dictionary<byte, Array<byte>> SyncData;

            for (int i = 0; i < entity->Scripts.Count(); ++i)
            {
                IComponent* comp = Cast<IComponent>(entity->Scripts[i]);
                if (comp && comp->Type == INetworkedObject::NetworkingType::Replicated)
                {
                    stream->Initialize();

                    if (INetworkSerializable* networked = ToInterface<INetworkSerializable>(comp))
                    {
                        networked->Serialize(stream);
                    }
                    else
                    {
                        NetworkReplicator::InvokeSerializer(comp->GetTypeHandle(), comp, stream, true);
                    }

                    SyncData[static_cast<byte>(i)] = { stream->GetBuffer(), static_cast<int32>(stream->GetPosition()) };
                }
            }

            MemoryWriteStream stream(32 * entity->Scripts.Count());

            stream.Write(SyncData);

            if (!EngineHelper::Compress(stream.GetHandle(), stream.GetPosition()))
            {
                UCRIT(true, "EngineHelper::Compress failed to compress replication data for entity \"{0}\".", entity->GetNamePath());
                return;
            }

            NetworkRpcParams params;
            uint32 ids[1] = { sync.client->ClientId };
            params.TargetIds = ToSpan(ids, ARRAY_COUNT(ids));

            uint32 resultLength = 0;

            // We should only send the compressed version if we are certain that it would be benificial to the bandwith
            if (stream.GetPosition() < static_cast<uint32>(EngineHelper::_compressBuffer.Count()))
            {
                resultLength = stream.GetPosition();
                entity->SendData(false, { stream.GetHandle(), static_cast<int32>(stream.GetPosition()) }, 0, params);
            }
            else
            {
                resultLength = EngineHelper::_compressBuffer.Count();
                entity->SendData(true, EngineHelper::_compressBuffer, stream.GetPosition(), params);
            }

            if (resultLength > 250)
            {
                UWARN("Entity \"{0}\" requires {1} bytes for initial replication! Not good!", entity->GetNamePath(), resultLength);
            }
        }
        // We are dealing with a replicated INetworkedObject, which means it is probably a System or something simular 
        // (where hierarchy does not matter and grouping like in entity code is probably out of the question)
        else
        {
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
            if (ScriptingObjectReference<INetworkedObject> object = Cast<INetworkedObject>(sync.object))
            {
                uint32 resultLength = 0;

                // We should only send the compressed version if we are certain that it would be benificial to the bandwith
                if (stream->GetLength() < static_cast<uint32>(EngineHelper::_compressBuffer.Count()))
                {
                    resultLength = stream->GetLength();
                    entity->SendData(false, { stream->GetBuffer(), static_cast<int32>(stream->GetLength()) }, 0, params);
                }
                else
                {
                    resultLength = EngineHelper::_compressBuffer.Count();
                    entity->SendData(true, EngineHelper::_compressBuffer, stream->GetLength(), params);
                }

                if (resultLength > 250)
                {
                    UWARN("INetworkedObject \"{0}\" requires {1} bytes for initial replication! Not good!", object->GetActor()->GetNamePath(), resultLength);
                }
            }
        }
    }
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
            if (comp->Type != INetworkedObject::NetworkingType::None)
            {
                NetworkReplicator::SpawnObject(target->Scripts[i]);
            }
            if (NetworkManager::IsHost() && comp->Type == INetworkedObject::NetworkingType::Replicated)
            {
                _spawnList[target->Priority].Add(Cast<ScriptingObject>(target));
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
    if (target->Type != Entity::NetworkingType::None)
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
            if (NetworkManager::IsHost() && comp->Type == INetworkedObject::NetworkingType::Replicated)
            {
                _spawnList[target->Priority].Remove(Cast<ScriptingObject>(target));
            }
            if (comp->Type != INetworkedObject::NetworkingType::None)
            {
                NetworkReplicator::DespawnObject(target->Scripts[i]);
            }
        }
    }

    NetworkReplicator::DespawnObject(target);
}

void Networking::StartReplicating(ScriptingObjectReference<Entity> target)
{
    NetworkReplicator::AddObject(target);
    for (int i = 0; i < target->Scripts.Count(); ++i)
    {
        IComponent* comp = Cast<IComponent>(target->Scripts[i]);
        if (comp)
        {
            if (comp->Type != INetworkedObject::NetworkingType::None)
            {
                NetworkReplicator::AddObject(target->Scripts[i]);
            }
            if (NetworkManager::IsHost() && comp->Type == INetworkedObject::NetworkingType::Replicated)
            {
                _spawnList[target->Priority].Add(Cast<ScriptingObject>(target));
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

void Networking::StopReplicating(ScriptingObjectReference<Entity> target)
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
            if (NetworkManager::IsHost() && comp->Type == INetworkedObject::NetworkingType::Replicated)
            {
                _spawnList[target->Priority].Remove(Cast<ScriptingObject>(target));
            }
            if (comp->Type != INetworkedObject::NetworkingType::None)
            {
                NetworkReplicator::RemoveObject(target->Scripts[i]);
            }
        }
    }
    NetworkReplicator::RemoveObject(target);
}

void Networking::DespawnReplicating(ScriptingObjectReference<Entity> target)
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
            if (NetworkManager::IsHost() && comp->Type == INetworkedObject::NetworkingType::Replicated)
            {
                _spawnList[target->Priority].Remove(Cast<ScriptingObject>(target));
            }
            if (comp->Type != INetworkedObject::NetworkingType::None)
            {
                NetworkReplicator::DespawnObject(target->Scripts[i]);
            }
        }
    }
    NetworkReplicator::DespawnObject(target);
}

Guid Networking::SerializeEntity(ScriptingObjectReference<Entity> target)
{
    if (PlayerOwned* owned = target->GetComponent<PlayerOwned>())
    {
        return owned->GetServerIdLocal();
    }
    return target->GetID();
}

ScriptingObjectReference<Entity> Networking::ConvertEntity(Guid id)
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