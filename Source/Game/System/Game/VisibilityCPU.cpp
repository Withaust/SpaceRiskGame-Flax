#include "VisibilityCPU.h"

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

bool VisibilityCPU::Queue(Vector3 Origin, Vector3 Direction)
{
	return Physics::RayCast(Origin, Direction, 5000.0f, RaycastMask, false);
}
