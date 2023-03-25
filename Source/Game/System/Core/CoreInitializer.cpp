#include "CoreInitializer.h"

CoreInitializer::CoreInitializer(const SpawnParams& params)
    : ISystem(params)
{

}

void CoreInitializer::OnInitialize()
{
    LevelManager::Get()->LoadLevel(TEXT("Game"));
    CoreInstance::Instance()->ReplicateSystems();
}

void CoreInitializer::OnDeinitialize()
{

}
