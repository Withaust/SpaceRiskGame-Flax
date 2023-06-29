#include "Networking.h"

UIMPL_SINGLETON(Networking)

Networking::Networking(const SpawnParams& params)
    : ISystem(params)
{
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

void Networking::AskForSync(NetworkRpcParams info)
{
    NETWORK_RPC_IMPL(Networking, AskForSync, info);
    if (_hierarchy)
    {
        _hierarchy->OnClientConnected(NetworkManager::GetClient(info.SenderId));
    }
}

void Networking::RequestSpawnSync()
{
    // TODO: https://github.com/FlaxEngine/FlaxEngine/issues/1211
    _syncFrame++;

    if (_syncFrame == 15)
    {
        AskForSync();   
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
}

void Networking::StartGame()
{
    _hierarchy = New<CustomHierarchy>();
    NetworkReplicator::SetHierarchy(_hierarchy);
    const Args* args = LaunchArgs::Instance->GetArgs();
    NetworkSettings* settings = NetworkSettings::Get();
    settings->NetworkFPS = 20.0f;
    settings->Address = args->Hostname;
    settings->Port = args->Port;
    settings->MaxClients = 6;
    settings->Apply();

    if (args->IsHost)
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
    ScriptingObjectReference<Entity> entity = Cast<Entity>(actor);
    if (!entity)
    {
        Logger::Instance->Error(TEXT("Tried to spawn prefab ") + actor->GetName() + TEXT(" but it was not an entity!"));
        return {};
    }

    NetworkReplicator::SpawnObject(entity);
    for (int i = 0; i < entity->Scripts.Count(); ++i)
    {
        NetworkReplicator::SpawnObject(entity->Scripts[i]);
    }

    if (NetworkManager::LocalClientId == ownerId)
    {
        NetworkReplicator::SetObjectOwnership(entity, ownerId, NetworkObjectRole::OwnedAuthoritative, true);
    }
    else
    {
        NetworkReplicator::SetObjectOwnership(entity, ownerId, NetworkObjectRole::ReplicatedSimulated, true);
    }

    Level::SpawnActor(entity, parent);

    entity->SetTransform(Transform(position, rotation));

    // Force executing on a spawned prefab even not on networked event
    for (int i = 0; i < entity->Scripts.Count(); ++i)
    {
        INetworkObject* netObj = ScriptingObject::ToInterface<INetworkObject>(entity->Scripts[i]);
        if (netObj)
        {
            netObj->OnNetworkSpawn();
        }
    }

    return entity;
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

    NetworkReplicator::DespawnObject(target);
}

void Networking::StartReplicating(Entity* target)
{
    NetworkReplicator::AddObject(target);
    for (int i = 0; i < target->Scripts.Count(); ++i)
    {
        NetworkReplicator::AddObject(target->Scripts[i]);
    }
    if (NetworkManager::LocalClientId == NetworkManager::ServerClientId)
    {
        NetworkReplicator::SetObjectOwnership(target, NetworkManager::ServerClientId, NetworkObjectRole::OwnedAuthoritative, true);
    }
    else
    {
        NetworkReplicator::SetObjectOwnership(target, NetworkManager::ServerClientId, NetworkObjectRole::ReplicatedSimulated, true);
    }
    INetworkObject* netObj = ScriptingObject::ToInterface<INetworkObject>(target);
    if (netObj)
    {
        netObj->OnNetworkSpawn();
    }
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
    INetworkObject* netObj = ScriptingObject::ToInterface<INetworkObject>(target);
    if (netObj)
    {
        netObj->OnNetworkDespawn();
    }
    for (int i = 0; i < target->Scripts.Count(); ++i)
    {
        netObj = ScriptingObject::ToInterface<INetworkObject>(target->Scripts[i]);
        if (netObj)
        {
            netObj->OnNetworkDespawn();
        }
    }
    for (int i = 0; i < target->Scripts.Count(); ++i)
    {
        NetworkReplicator::RemoveObject(target->Scripts[i]);
    }
    NetworkReplicator::RemoveObject(target);
}

void Networking::DespawnReplicating(Entity* target)
{
    target->Despawning = true;
    INetworkObject* netObj = ScriptingObject::ToInterface<INetworkObject>(target);
    if (netObj)
    {
        netObj->OnNetworkDespawn();
    }
    for (int i = 0; i < target->Scripts.Count(); ++i)
    {
        netObj = ScriptingObject::ToInterface<INetworkObject>(target->Scripts[i]);
        if (netObj)
        {
            netObj->OnNetworkDespawn();
        }
    }
    for (int i = 0; i < target->Scripts.Count(); ++i)
    {
        NetworkReplicator::DespawnObject(target->Scripts[i]);
    }
    NetworkReplicator::DespawnObject(target);
}
