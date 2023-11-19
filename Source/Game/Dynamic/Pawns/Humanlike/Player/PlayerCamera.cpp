#include "PlayerCamera.h"

PlayerCamera::PlayerCamera(const SpawnParams& params)
    : Script(params)
{
}

void PlayerCamera::OnEnable()
{
    if (!_movement)
    {
        _movement = Entity::FindEntity(this)->GetComponent<PlayerMovement>();
    }

    _movement->CanMove = true;
}

void PlayerCamera::OnDisable()
{
    if (_movement)
    {
        _movement->CanMove = false;
    }
}
