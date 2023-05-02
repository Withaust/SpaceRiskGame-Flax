#include "CoreInitializer.h"

CoreInitializer::CoreInitializer(const SpawnParams& params)
    : ISystem(params)
{
}

void CoreInitializer::OnInitialize()
{
    if(LaunchArgs::Get()->GetArgs()->IsHost)
    {
        LevelManager::Get()->LoadLevel(TEXT("Game"));
    }
    else
    {
        Networking::Get()->StartGame();
        CoreInstance::Instance()->ReplicateSystems();
    }
}

void CoreInitializer::OnDeinitialize()
{
}

void CoreInitializer::OnSceneLoaded(Scene* scene)
{
    if (LaunchArgs::Get()->GetArgs()->IsHost)
    {
        Networking::Get()->StartGame();
        CoreInstance::Instance()->ReplicateSystems();
    }
}
