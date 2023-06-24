#include "CoreInitializer.h"

UIMPL_SINGLETON(CoreInitializer)

CoreInitializer::CoreInitializer(const SpawnParams& params)
    : ISystem(params)
{
}

void CoreInitializer::OnInitialize()
{
    if (LaunchArgs::Instance->GetArgs()->IsHost)
    {
        String Level = TEXT("Main");
        if (SceneRoot::EditorLaunch)
        {
            DebugArgs* args = SceneRoot::EditorLaunch->GetInstance<DebugArgs>();
            if (args->LaunchScene != String::Empty)
            {
                Level = args->LaunchScene;
                args->LaunchScene = String::Empty;
            }
        }
        Networking::Instance->StartGame();
        LevelManager::Instance->LoadLevel(Level);
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
}

void CoreInitializer::OnSceneLoaded(Scene* scene)
{
    if (LaunchArgs::Instance->GetArgs()->IsHost)
    {
        Core::Instance->OnPlayerConnected(NetworkManager::LocalClient);
        Core::Instance->ReplicateSystems();
        UI::Instance->GoForward(TEXT("Game.UIGame"));
    }
}
