#include "RobotInteractor.h"

RobotInteractor::RobotInteractor(const SpawnParams& params)
    : IComponent(params)
{
}

void RobotInteractor::OnNetworkSpawn()
{
    UNOT_OWNED_RETURN();

    String entityPath = GetEntity()->GetNamePath();
    RandomStream rand(StringUtils::GetHashCode(entityPath.GetText(), entityPath.Length()));

    InteractionTarget = Guid::New();
    InteractionType = rand.RandRange(0, 255);
}
