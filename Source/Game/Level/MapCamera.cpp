#include "MapCamera.h"

MapCamera::MapCamera(const SpawnParams& params)
    : Script(params)
{
}

void MapCamera::OnEnable()
{
    Camera* camera = Cast<Camera>(GetActor());
    if (!Map::Instance || !camera)
    {
        return;
    }

    Map::Instance->Camera = camera;

    if (!_everDisabled)
    {
        GetActor()->SetIsActive(false);
        _everDisabled = true;
    }
}

void MapCamera::OnDisable()
{

}
