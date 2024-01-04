#include "RobotAI.h"

RobotAI::RobotAI(const SpawnParams& params)
    : IComponent(params)
{
}

void RobotAI::OnNetworkSpawn()
{
    UNOT_OWNED_RETURN();

    String entityPath = GetEntity()->GetNamePath();
    RandomStream rand(StringUtils::GetHashCode(entityPath.GetText(), entityPath.Length()));
    State1 = rand.GetUnsignedInt();
    State2 = rand.GetUnsignedInt();
    State3 = rand.GetUnsignedInt();
}
