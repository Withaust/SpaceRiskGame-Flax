#include "BoxColliderVisual.h"

BoxColliderVisual::BoxColliderVisual(const SpawnParams& params)
    : BoxCollider(params)
{
}

#if USE_EDITOR
void BoxColliderVisual::OnDebugDraw()
{
    DEBUG_DRAW_BOX(GetOrientedBox(), Color, 0, true);

    // Base
    Collider::OnDebugDraw();
}

void BoxColliderVisual::OnDebugDrawSelected()
{
    DEBUG_DRAW_WIRE_BOX(GetOrientedBox(), OutlineColor, 0, true);
    DEBUG_DRAW_BOX(GetOrientedBox(), Color, 0, true);

    // Base
    Collider::OnDebugDrawSelected();
}
#endif
