#include "CoreInitializer.h"

CoreInitializer::CoreInitializer(const SpawnParams& params)
    : ISystem(params)
{
}

void CoreInitializer::OnInitialize()
{
    if (Core::Instance()->Get<LaunchArgs>()->GetArgs()->IsHost)
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
        Core::Instance()->Get<LevelManager>()->LoadLevel(Level);
    }
    else
    {
        Core::Instance()->Get<Networking>()->StartGame();
        Core::Instance()->ReplicateSystems();
        Core::Instance()->Get<UI>()->GoForward(TEXT("Game.UIGame"));
    }
}

void CoreInitializer::OnDeinitialize()
{
}

void CoreInitializer::OnSceneLoaded(Scene* scene)
{
    if (Core::Get<LaunchArgs>()->GetArgs()->IsHost)
    {
        Core::Get<Networking>()->StartGame();
        Core::Instance()->ReplicateSystems();
        Core::Get<UI>()->GoForward(TEXT("Game.UIGame"));
    }
}
