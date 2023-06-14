#include "Entity.h"

bool Entity::proxyAdded = false;

Entity::Entity(const SpawnParams& params)
    : Actor(params)
{

}

#if USE_EDITOR

// SceneGraphFactory.CustomNodesTypes is not exposed to C++, so
// we have to get access to it thought reflections of C#
void Entity::CheckProxy()
{
    if (proxyAdded)
    {
        return;
    }
    proxyAdded = true;
    for (const auto& type : GetType().Module->Types)
    {
        if (type.Fullname != "Game.EntityProxy")
        {
            continue;
        }
        type.ManagedClass->GetMethod("Add")->Invoke(nullptr, nullptr, nullptr);
    }
}

void Entity::OnDebugDrawSelected()
{
    Actor::OnDebugDrawSelected();
    if (Icon.GetID() != iconId)
    {
        CheckProxy();
        ViewportIconsRenderer::RemoveActor(this);
        ViewportIconsRenderer::AddActorWithTexture(this, Icon);
        iconId = Icon.GetID();
    }
}

void Entity::OnDisable()
{
    if (iconId != Guid::Empty)
    {
        CheckProxy();
        ViewportIconsRenderer::RemoveActor(this);
    }
    Actor::OnDisable();
}
#endif

void Entity::OnEnable()
{
    Actor::OnEnable();
#if USE_EDITOR
    if (Icon)
    {
        CheckProxy();
        ViewportIconsRenderer::AddActorWithTexture(this, Icon);
        iconId = Icon.GetID();
    }
#endif

    // TODO Parse and add scripts to component list
}
