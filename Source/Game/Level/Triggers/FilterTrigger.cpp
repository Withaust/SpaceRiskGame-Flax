#include "FilterTrigger.h"

FilterTrigger::FilterTrigger(const SpawnParams& params)
    : IComponent(params)
{
}

void FilterTrigger::OnTriggerEntry(PhysicsColliderActor* actor)
{
    Entity* entity = Entity::FindEntity(actor);
    if (entity && entity->Tags == FilterTags)
    {
        OnTriggerEnter(entity);
    }
}

void FilterTrigger::OnTriggerExit(PhysicsColliderActor* actor)
{
    Entity* entity = Entity::FindEntity(actor);
    if (entity && entity->Tags == FilterTags)
    {
        OnTriggerLeave(entity);
    }
}

void FilterTrigger::OnEnable()
{
    Collider->TriggerEnter.Bind<FilterTrigger, &FilterTrigger::OnTriggerEntry>(this);
    Collider->TriggerExit.Bind<FilterTrigger, &FilterTrigger::OnTriggerExit>(this);
}

void FilterTrigger::OnDisable()
{
    Collider->TriggerEnter.Unbind<FilterTrigger, &FilterTrigger::OnTriggerEntry>(this);
    Collider->TriggerExit.Unbind<FilterTrigger, &FilterTrigger::OnTriggerExit>(this);
}
