#include "CapsuleColliderVisual.h"

CapsuleColliderVisual::CapsuleColliderVisual(const SpawnParams& params)
    : CapsuleCollider(params)
{
}

#if USE_EDITOR
void CapsuleColliderVisual::OnDebugDraw()
{
    Quaternion rot;
    Quaternion::Multiply(_transform.Orientation, Quaternion::Euler(0, 90, 0), rot);
    const float scaling = _cachedScale.GetAbsolute().MaxValue();
    const float minSize = 0.001f;
    const float radius = Math::Max(Math::Abs(GetRadius()) * scaling, minSize);
    const float height = Math::Max(Math::Abs(GetHeight()) * scaling, minSize);
    DEBUG_DRAW_TUBE(_transform.LocalToWorld(_center), rot, radius, height, Color, 0, false);

    // Base
    Collider::OnDebugDraw();
}

void CapsuleColliderVisual::OnDebugDrawSelected()
{
    Quaternion rot;
    Quaternion::Multiply(_transform.Orientation, Quaternion::Euler(0, 90, 0), rot);
    const float scaling = _cachedScale.GetAbsolute().MaxValue();
    const float minSize = 0.001f;
    const float radius = Math::Max(Math::Abs(GetRadius()) * scaling, minSize);
    const float height = Math::Max(Math::Abs(GetHeight()) * scaling, minSize);
    DEBUG_DRAW_WIRE_TUBE(_transform.LocalToWorld(_center), rot, radius, height, OutlineColor, 0, false);
    DEBUG_DRAW_TUBE(_transform.LocalToWorld(_center), rot, radius, height, Color, 0, false);

    // Base
    Collider::OnDebugDrawSelected();
}
#endif
