#include "VisibilityCPU.h"

UIMPL_SINGLETON(VisibilityCPU)

VisibilityCPU::VisibilityCPU(const SpawnParams& params)
    : ISystem(params)
{
}

void VisibilityCPU::OnInitialize()
{
}

void VisibilityCPU::OnDeinitialize()
{
}

bool VisibilityCPU::Queue(Vector3 origin, Vector3 direction)
{
    return Physics::RayCast(origin, direction, 5000.0f, RaycastMask, false);
}
