#include "RobotRespawns.h"

UIMPL_SINGLETON(RobotRespawns)

RobotRespawns::RobotRespawns(const SpawnParams& params)
    : ISystem(params)
{
}

void RobotRespawns::OnPlayerConnected(NetworkClient* client)
{
    if (_fired)
    {
        return;
    }

    for (int x = 0; x < 15; x++)
    {
        for (int y = 0; y < 15; y++)
        {
            const Vector3 target((float)x * 100.0f, 0.0f, (float)y * 100.0f);

            Networking::Instance->SpawnPrefab(RobotPrefab, GetActor(), 0, target);
        }
    }

    _fired = true;
}

void RobotRespawns::OnDeinitialize()
{
    UDEINIT_SINGLETON();
}
