#include "Networking.h"

Networking::Networking(const SpawnParams& params)
    : ISystem(params)
{
}

bool Networking::CheckImmediateOwnership(ScriptingObject* target)
{
    return _immediateOwner.Contains(target);
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
        Core::Instance()->OnConnected();
        break;
    }
    case NetworkConnectionState::Offline:
    case NetworkConnectionState::Disconnected:
    {
        Core::Instance()->OnDisconnected();
        break;
    }
    }
}

void Networking::OnNetworkClientConnected(NetworkClient* client)
{
    Core::Instance()->OnPlayerConnected(client);
}

void Networking::OnNetworkClientDisconnected(NetworkClient* client)
{
    Core::Instance()->OnPlayerDisconnected(client);
}

void Networking::OnInitialize()
{
#if !BUILD_RELEASE
    NetworkReplicator::EnableLog = true;
#endif
    NetworkManager::StateChanged.Bind<Networking, &Networking::OnNetworkStateChanged>(this);
    NetworkManager::ClientConnected.Bind<Networking, &Networking::OnNetworkClientConnected>(this);
    NetworkManager::ClientDisconnected.Bind<Networking, &Networking::OnNetworkClientDisconnected>(this);
}

void Networking::OnDeinitialize()
{
    NetworkManager::StateChanged.Unbind<Networking, &Networking::OnNetworkStateChanged>(this);
    NetworkManager::ClientConnected.Unbind<Networking, &Networking::OnNetworkClientConnected>(this);
    NetworkManager::ClientDisconnected.Unbind<Networking, &Networking::OnNetworkClientDisconnected>(this);
}

void Networking::StartGame()
{
    const Args* args = Core::Get<LaunchArgs>()->GetArgs();
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

Actor* Networking::SpawnPrefab(Prefab* prefab, Actor* parent, uint32 ownerId, const Vector3& position, const Quaternion& rotation)
{
    Actor* newActor = PrefabManager::SpawnPrefab(prefab, parent);

    NetworkReplicator::SpawnObject(newActor);
    for (int i = 0; i < newActor->Scripts.Count(); ++i)
    {
        NetworkReplicator::SpawnObject(newActor->Scripts[i]);
    }

    if (NetworkManager::LocalClientId == ownerId)
    {
        NetworkReplicator::SetObjectOwnership(newActor, ownerId, NetworkObjectRole::OwnedAuthoritative, true);
        _immediateOwner.Add(newActor);
    }
    else
    {
        NetworkReplicator::SetObjectOwnership(newActor, ownerId, NetworkObjectRole::ReplicatedSimulated, true);
    }

    Level::SpawnActor(newActor, parent);

    newActor->SetPosition(position);
    newActor->SetOrientation(rotation);

    // Force executing on a spawned prefab even not on networked event
    for (int i = 0; i < newActor->Scripts.Count(); ++i)
    {
        INetworkObject* netObj = ScriptingObject::ToInterface<INetworkObject>(newActor->Scripts[i]);
        if (netObj)
        {
            netObj->OnNetworkSpawn();
        }
    }

    return newActor;
}

void Networking::DespawnPrefab(Actor* target)
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
    _immediateOwner.Remove(target);
}

void Networking::StartReplicating(ScriptingObject* target)
{
    NetworkReplicator::AddObject(target);
    INetworkObject* netObj = ScriptingObject::ToInterface<INetworkObject>(target);
    if (netObj)
    {
        netObj->OnNetworkSpawn();
    }
}

void Networking::StopReplicating(ScriptingObject* target)
{
    INetworkObject* netObj = ScriptingObject::ToInterface<INetworkObject>(target);
    if (netObj)
    {
        netObj->OnNetworkDespawn();
    }
    NetworkReplicator::RemoveObject(target);
}

void Networking::DespawnReplicating(ScriptingObject* target)
{
    INetworkObject* netObj = ScriptingObject::ToInterface<INetworkObject>(target);
    if (netObj)
    {
        netObj->OnNetworkDespawn();
    }
    NetworkReplicator::DespawnObject(target);
}
