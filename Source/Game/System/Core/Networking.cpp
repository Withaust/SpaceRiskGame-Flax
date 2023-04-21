#include "Networking.h"

Networking::Networking(const SpawnParams& params)
    : ISystem(params)
{
}

void Networking::OnNetworkStateChanged()
{
    UPRINT("OnNetworkStateChanged: {0}", ScriptingEnum::ToString(NetworkManager::State));

    switch (NetworkManager::State)
    {
    case NetworkConnectionState::Connected:
    {
#if !BUILD_RELEASE
        if (Engine::MainWindow)
        {
            _windowTitle = Engine::MainWindow->GetTitle();
            Engine::MainWindow->SetTitle(String::Format(TEXT("{} - {} Id {}"), _windowTitle, NetworkManager::IsClient() ? TEXT("Client") : TEXT("Host"), NetworkManager::LocalClientId));
        }
#endif
        break;
    }
    case NetworkConnectionState::Offline:
    case NetworkConnectionState::Disconnected:
    {
#if !BUILD_RELEASE
        if (Engine::MainWindow)
        {
            Engine::MainWindow->SetTitle(_windowTitle);
        }
#endif
        break;
    }
    }
}

void Networking::OnNetworkClientConnected(NetworkClient* client)
{
    CoreInstance::Instance()->OnPlayerConnected(client);
}

void Networking::OnNetworkClientDisconnected(NetworkClient* client)
{
    CoreInstance::Instance()->OnPlayerDisconnected(client);
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

void Networking::OnSceneLoaded(Scene* scene)
{
}

void Networking::OnSceneUnloaded(Scene* scene)
{
}

void Networking::StartGame()
{
    const Args* args = LaunchArgs::Get()->GetArgs();
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
