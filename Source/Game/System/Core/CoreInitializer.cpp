#include "CoreInitializer.h"

CoreInitializer::CoreInitializer(const SpawnParams& params)
    : ISystem(params)
{
}

void CoreInitializer::OnInitialize()
{
    if (Core::Get<LaunchArgs>()->GetArgs()->IsHost)
    {
        Core::Get<LevelManager>()->LoadLevel(TEXT("Game"));
    }
    else
    {
        Core::Get<Networking>()->StartGame();
        Core::Instance()->ReplicateSystems();
        Core::Get<UI>()->GoForward(TEXT("Game.UIGame"));
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
