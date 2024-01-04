#include "RobotSquad.h"

RobotSquad::RobotSquad(const SpawnParams& params)
    : IComponent(params)
{
}

void RobotSquad::OnNetworkSpawn()
{
    UNOT_OWNED_RETURN();

    SquadId = Guid::New();
}
