#include "CoreInitializer.h"

CoreInitializer::CoreInitializer(const SpawnParams& params)
    : ISystem(params)
{

}

void CoreInitializer::OnInitialize()
{
    LevelManager::Get()->LoadLevel(TEXT("Game"));
    CoreInstance::GetInstance()->ReplicateSystems();
}

void CoreInitializer::OnDeinitialize()
{

}
