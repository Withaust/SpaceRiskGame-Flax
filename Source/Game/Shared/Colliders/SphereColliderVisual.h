#pragma once

#if USE_EDITOR
#include <Engine/Core/Math/Color.h>
#include <Engine/Debug/DebugDraw.h>
#include <Engine/Physics/Colliders/Collider.h>
#endif
#include <Engine/Physics/Colliders/SphereCollider.h>

API_CLASS(Attributes = "ActorContextMenu(\"New/Physics/Colliders/Sphere Collider Visual\"), ActorToolbox(\"Physics\")")
class GAME_API SphereColliderVisual : public SphereCollider
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(SphereColliderVisual);
public:
#if USE_EDITOR
    API_FIELD() Color OutlineColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    API_FIELD() Color Color = { 1.0f, 0.50f, 0.15f, 0.2f };
    void OnDebugDraw() override;
    void OnDebugDrawSelected() override;
#endif
};
