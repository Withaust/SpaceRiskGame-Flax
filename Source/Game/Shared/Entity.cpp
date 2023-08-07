#include "Entity.h"
#include <Game/System/Core/Networking/Networking.h>

#if USE_EDITOR
bool Entity::proxyAdded = false;
#endif

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

            if (Components.ContainsKey(itteratorHandle.GetType().Fullname))
            {
                Logger::Instance->Error(String::Format(TEXT("{0} contains a script {1} which reimplements interface {2}"),
                    GetName().GetText(), targetHandle.GetType().Fullname.ToString().GetText(), itteratorHandle.GetType().Fullname.ToString().GetText()));
                break;
            }

            Components[itteratorHandle.GetType().Fullname] = component;

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
    if (Icon && Icon.GetID() != iconId && GetScene())
    {
        CheckProxy();
        ViewportIconsRenderer::RemoveActor(this);
        ViewportIconsRenderer::AddActorWithTexture(this, Icon);
        iconId = Icon.GetID();
    }
}
#endif

void Entity::OnDisable()
{
#if USE_EDITOR
    if (iconId != Guid::Empty && GetScene())
    {
        CheckProxy();
        ViewportIconsRenderer::RemoveActor(this);
    }
    if (!Editor::IsPlayMode)
    {
        Actor::OnDisable();
        return;
    }
#endif
    if (LevelReplication == EntityType::Networked && !Despawning)
    {
        Networking::Instance->StopReplicating(this);
    }
    Actor::OnDisable();
}

void Entity::OnEnable()
{
    Actor::OnEnable();
#if USE_EDITOR
    if (Icon && GetScene())
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

    if (LevelReplication == EntityType::Networked)
    {
        Networking::Instance->StartReplicating(this);
    }
    else if (NetworkManager::IsClient() && LevelReplication == EntityType::HostOnly)
    {
        DeleteObject();
        return;
    }
}

const Dictionary<String, IComponent*> Entity::GetComponents() const
{
    Dictionary<String, IComponent*> result;
    for (const auto& entry : Components)
    {
        result[entry.Key.ToString()] = entry.Value;
    }
    return result;
}

ScriptingObjectReference<IComponent> Entity::GetComponent(const MClass* type)
{
    if (!GotComponents)
    {
        CacheComponents();
        GotComponents = true;
    }
    return Components[type->GetFullName()];
}

ScriptingObjectReference<IComponent> Entity::GetComponent(const ScriptingTypeHandle& type)
{
    if (!GotComponents)
    {
        CacheComponents();
        GotComponents = true;
    }
    return Components[type.GetType().Fullname];
}
