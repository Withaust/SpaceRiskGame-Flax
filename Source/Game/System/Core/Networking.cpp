#include "Networking.h"

Networking::Networking(const SpawnParams& params)
    : ISystem(params)
{

}

void Networking::OnUpdate()
{

}

void Networking::OnNetworkStateChanged()
{

}

void Networking::OnNetworkClientConnected(NetworkClient* client)
{

}

void Networking::OnNetworkClientDisconnected(NetworkClient* client)
{

}

void Networking::OnInitialize()
{
    Engine::Update.Bind<Networking, &Networking::OnUpdate>(this);
    NetworkManager::StateChanged.Bind<Networking, &Networking::OnNetworkStateChanged>(this);
    NetworkManager::ClientConnected.Bind<Networking, &Networking::OnNetworkClientConnected>(this);
    NetworkManager::ClientDisconnected.Bind<Networking, &Networking::OnNetworkClientDisconnected>(this);
}

void Networking::OnDeinitialize()
{
    Engine::Update.Unbind<Networking, &Networking::OnUpdate>(this);
    NetworkManager::StateChanged.Unbind<Networking, &Networking::OnNetworkStateChanged>(this);
    NetworkManager::ClientConnected.Unbind<Networking, &Networking::OnNetworkClientConnected>(this);
    NetworkManager::ClientDisconnected.Unbind<Networking, &Networking::OnNetworkClientDisconnected>(this);
}   

void Networking::OnSceneLoaded(Scene* scene)
{

}

void Networking::OnSceneUnloaded(Scene* scene)
{

}

void Networking::StartGame()
{
    const Args* Args = GET_SYSTEM(LaunchArgs)->GetArgs();
    NetworkSettings* Settings = NetworkSettings::Get();
    Settings->NetworkFPS = 20.0f;
    Settings->Address = Args->Hostname;
    Settings->Port = Args->Port;
    Settings->MaxClients = 6;
    Settings->Apply();

    if (Args->IsHost)
    {
        NetworkManager::StartHost();
    }
    else
    {
        NetworkManager::StartClient();
    }

    if (NetworkManager::Mode == NetworkManagerMode::Host)
    {
        OnNetworkClientConnected(NetworkManager::LocalClient);
    }
}