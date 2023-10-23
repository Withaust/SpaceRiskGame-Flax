#include "CoreInitializer.h"

UIMPL_SINGLETON(CoreInitializer)

CoreInitializer::CoreInitializer(const SpawnParams& params)
    : ISystem(params)
{
}

void CoreInitializer::OnInitialize()
{
    if (LaunchArgs::Instance->IsHost)
    {
        Networking::Instance->StartGame();
        LevelManager::Instance->LoadLevel(LaunchArgs::Instance->Level);
        Networking::Instance->BindEvents();
    }
    else
    {
        Networking::Instance->BindEvents();
        Networking::Instance->StartGame();
        Core::Instance->ReplicateSystems();
        UI::Instance->GoForward(TEXT("Game.UIGame"));
    }
}

void CoreInitializer::OnDeinitialize()
{
    UDEINIT_SINGLETON();
}

void CoreInitializer::OnSceneLoaded(Scene* scene)
{
    if (LaunchArgs::Instance->IsHost)
    {
        Core::Instance->OnPlayerConnected(NetworkManager::LocalClient);
        Core::Instance->ReplicateSystems();
        UI::Instance->GoForward(TEXT("Game.UIGame"));
    }
}
