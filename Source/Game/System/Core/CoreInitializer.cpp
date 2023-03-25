#include "CoreInitializer.h"

CoreInitializer::CoreInitializer(const SpawnParams& params)
    : ISystem(params)
{

}

void CoreInitializer::OnInitialize()
{
    GET_SYSTEM(LevelManager)->LoadLevel(TEXT("Game"));
    CoreInstance::GetInstance()->ReplicateSystems();
}

void CoreInitializer::OnDeinitialize()
{

}
