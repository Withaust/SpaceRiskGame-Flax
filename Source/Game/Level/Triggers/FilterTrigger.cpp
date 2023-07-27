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
        if (!Containing.ContainsKey(entity))
        {
            Containing[entity] = 1;
            OnTriggerEnter(entity);
        }
        else
        {
            Containing.At(entity)++;
        }
    }
}

void FilterTrigger::OnTriggerExit(PhysicsColliderActor* actor)
{
    Entity* entity = Entity::FindEntity(actor);
    if (entity && entity->Tags == FilterTags)
    {
        uint32& count = Containing.At(entity);
        count--;
        if (count == 0)
        {
            Containing.Remove(entity);
            OnTriggerLeave(entity);
        }
    }
}

void FilterTrigger::OnEnable()
{
    for (const auto& child : GetEntity()->Children)
    {
        Collider* collider = Cast<Collider>(child);
        if (!collider)
        {
            continue;
        }
        collider->TriggerEnter.Bind<FilterTrigger, &FilterTrigger::OnTriggerEntry>(this);
        collider->TriggerExit.Bind<FilterTrigger, &FilterTrigger::OnTriggerExit>(this);
    }
}

void FilterTrigger::OnDisable()
{
    for (const auto& child : GetEntity()->Children)
    {
        Collider* collider = Cast<Collider>(child);
        if (!collider)
        {
            continue;
        }
        collider->TriggerEnter.Unbind<FilterTrigger, &FilterTrigger::OnTriggerEntry>(this);
        collider->TriggerExit.Unbind<FilterTrigger, &FilterTrigger::OnTriggerExit>(this);
    }
}

#if USE_EDITOR
void FilterTrigger::OnDebugDrawSelected()
{
    for (const auto& child : GetEntity()->Children)
    {
        Collider* collider = Cast<Collider>(child);
        if (collider)
        {
            collider->OnDebugDrawSelected();
        }
    }
}
#endif