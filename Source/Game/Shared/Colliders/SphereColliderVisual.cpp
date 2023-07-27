#include "SphereColliderVisual.h"

SphereColliderVisual::SphereColliderVisual(const SpawnParams& params)
    : SphereCollider(params)
{
}

#if USE_EDITOR
void SphereColliderVisual::OnDebugDraw()
{
    DEBUG_DRAW_SPHERE(_sphere, Color, 0, false);

    // Base
    Collider::OnDebugDraw();
}

void SphereColliderVisual::OnDebugDrawSelected()
{
    DEBUG_DRAW_WIRE_SPHERE(_sphere, OutlineColor, 0, false);
    DEBUG_DRAW_SPHERE(_sphere, Color, 0, false);

    // Base
    Collider::OnDebugDrawSelected();
}
#endif
