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
            EditorLaunchArgs* args = SceneRoot::EditorLaunch->GetInstance<EditorLaunchArgs>();
            if (args->LaunchScene != String::Empty)
            {
                Level = args->LaunchScene;
                args->LaunchScene = String::Empty;
            }
        }
        LevelManager::Instance->LoadLevel(Level);
    }
    else
    {
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
        Networking::Instance->StartGame();
        Core::Instance->ReplicateSystems();
        UI::Instance->GoForward(TEXT("Game.UIGame"));
    }
}
