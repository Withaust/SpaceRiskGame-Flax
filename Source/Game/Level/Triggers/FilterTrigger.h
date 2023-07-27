#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actor.h>
#include <Engine/Physics/Colliders/Collider.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/IComponent.h>

API_CLASS() class GAME_API FilterTrigger : public IComponent
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(FilterTrigger);
private:

    void OnTriggerEntry(PhysicsColliderActor* actor);
    void OnTriggerExit(PhysicsColliderActor* actor);
    Dictionary<Entity*, uint32> Containing;

public:
    API_FIELD() Array<Tag> FilterTags;

    API_EVENT() Delegate<Entity*> OnTriggerEnter;
    API_EVENT() Delegate<Entity*> OnTriggerLeave;

    void OnEnable() override;
    void OnDisable() override;
#if USE_EDITOR
    void OnDebugDrawSelected() override;
#endif
};

