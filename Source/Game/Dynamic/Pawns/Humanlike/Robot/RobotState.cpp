#include "RobotState.h"

RobotState::RobotState(const SpawnParams& params)
    : IComponent(params)
{
}

void RobotState::OnNetworkSpawn()
{
    if (!NetworkReplicator::IsObjectOwned(GetEntity())) { return; }

    String entityPath = GetEntity()->GetNamePath();
    RandomStream rand(StringUtils::GetHashCode(entityPath.GetText(), entityPath.Length()));

    StateId = rand.RandRange(0, 255);
}
