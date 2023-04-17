#include "CoreInitializer.h"

CoreInitializer::CoreInitializer(const SpawnParams& params)
    : ISystem(params)
{

}

void CoreInitializer::OnInitialize()
{
    LevelManager::Get()->LoadLevel(TEXT("Game"));    
}

void CoreInitializer::OnDeinitialize()
{

}

void CoreInitializer::OnSceneLoaded(Scene* scene)
{
    Networking::Get()->StartGame();
    CoreInstance::Instance()->ReplicateSystems();
}
