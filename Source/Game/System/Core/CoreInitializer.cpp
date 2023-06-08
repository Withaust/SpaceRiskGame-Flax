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
        if (SceneRoot::EditorScene != String::Empty)
        {
            Level = SceneRoot::EditorScene;
            // Editor does not perform full domain reload, so static
            // members never get reset without manual help
            SceneRoot::EditorScene = String::Empty;
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
