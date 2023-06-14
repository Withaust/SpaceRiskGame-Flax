#include "Entity.h"

bool Entity::proxyAdded = false;

Entity::Entity(const SpawnParams& params)
    : Actor(params)
{
}

void Entity::CacheComponents()
{
    const auto& ScriptHandle = Script::GetStaticType().GetHandle();
    const auto& ComponentHandle = IComponent::GetStaticType().GetHandle();

    for (const auto& script : Scripts)
    {
        IComponent* component = Cast<IComponent>(script);

        if (!component)
        {
            Logger::Instance->Error(String::Format(TEXT("{0} tried to pass script {1} as a Component, tough luck"),
                GetName(), script->GetTypeHandle().GetType().Fullname.ToString()));
            continue;
        }

        ScriptingTypeHandle targetHandle = script->GetTypeHandle();
        ScriptingTypeHandle itteratorHandle = targetHandle;

        while (true)
        {
            if (!itteratorHandle)
            {
                break;
            }

            if (itteratorHandle == ScriptHandle || itteratorHandle == ComponentHandle)
            {
                break;
            }

            if (Components.ContainsKey(itteratorHandle))
            {
                Logger::Instance->Error(String::Format(TEXT("{0} contains a script {1} which reimplements interface {2}"),
                    GetName(), targetHandle.GetType().Fullname.ToString(), itteratorHandle.GetType().Fullname.ToString()));
                break;
            }

            Components[itteratorHandle] = component;

            itteratorHandle = itteratorHandle.GetType().GetBaseType();
        }
    }
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

    if (!Editor::IsPlayMode)
    {
        return;
    }
#endif
}
