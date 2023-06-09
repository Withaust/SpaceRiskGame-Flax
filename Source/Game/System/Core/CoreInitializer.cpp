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
            Level = SceneRoot::EditorLaunch->GetInstance<EditorLaunchArgs>()->LaunchScene;
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
