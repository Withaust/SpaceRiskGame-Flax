#include "Footsteps.h"

Footsteps::Footsteps(const SpawnParams& params)
    : IComponent(params)
{
    _tickUpdate = true;
}

void Footsteps::OnEnable()
{
    _sleep = 2.0f;
}

void Footsteps::OnUpdate()
{
    if (USLEEP(_sleep))
    {
        RayCastHit hitInfo;
        Vector3 position = RaycastOrigin->GetPosition();
        Vector3 direction(0.0f, -1.0f, 0.0f);

        if (Physics::RayCast(position, direction, hitInfo, 100.0f, RaycastMask, false))
        {
            Terrain* terrain = Cast<Terrain>(hitInfo.Collider);

            if (!terrain)
            {
                return;
            }

            TestTerrain* test = terrain->GetScript<TestTerrain>();

            if (!test)
            {
                return;
            }

            auto type = test->GetTerrainType(position, direction, 100.0f);
            //UPRINT("{0}", ScriptingEnum::ToString(type));
        }
    }
}
