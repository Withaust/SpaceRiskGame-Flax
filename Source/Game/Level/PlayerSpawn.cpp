#include "PlayerSpawn.h"

PlayerSpawn::PlayerSpawn(const SpawnParams& params)
    : Script(params)
{
}
#if USE_EDITOR
void PlayerSpawn::OnDebugDraw()
{
    const Transform& trans = GetActor()->GetTransform();
    DebugDraw::DrawWireTube(trans.Translation, trans.Orientation * Quaternion::Euler(90, 0, 0), PlayerMovement::Radius, PlayerMovement::Height, Color::Green);
}
#endif
