#include "Entity.h"
#include <Game/System/Core/Networking.h>

#if USE_EDITOR
bool Entity::proxyAdded = false;
#endif

Entity::Entity(const SpawnParams& params)
    : Actor(params)
{
}

void Entity::CacheComponents()
{
    if (GotComponents)
    {
        return;
    }
    GotComponents = true;

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

void Entity::SendData(bool compressed, const Array<byte>& data, uint32 srcSize, NetworkRpcParams info)
{
    NETWORK_RPC_IMPL(Entity, SendData, compressed, data, srcSize, info);

    MemoryReadStream readStream;

    if (compressed && srcSize != 0)
    {
        if (!EngineHelper::Decompress(data, srcSize))
        {
            UCRIT(true, "EngineHelper::Decompress failed to decompress a replication data for entity \"{0}\".", GetNamePath());
            return;
        }

        const Array<byte>& buffer = EngineHelper::GetCompressBuffer();

        readStream.Init(buffer.begin(), buffer.Count());
    }
    else
    {
        readStream.Init(data.begin(), data.Count());
    }

    Dictionary<byte, Array<byte>> SyncData;

    readStream.Read(SyncData);

    NetworkStream* stream = Networking::Instance->_stream;

    for (const auto& comp : SyncData)
    {
        Script* target = GetScript(comp.Key);

        if (!target)
        {
            UCRIT(true, "Entity::SendData came for a missing/non-existent component {0} in entity \"{1}\".", comp.Key, GetNamePath());
            return;
        }

        stream->Initialize((byte*)comp.Value.begin(), comp.Value.Count());

        if (INetworkSerializable* networked = ToInterface<INetworkSerializable>(target))
        {
            networked->Deserialize(stream);
        }
        else
        {
            NetworkReplicator::InvokeSerializer(target->GetTypeHandle(), target, stream, false);
        }

        if (INetworkObject* netObj = ScriptingObject::ToInterface<INetworkObject>(target))
        {
            netObj->OnNetworkSync();
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
    if (Editor::IsPlayMode)
    {
        return;
    }

    Actor::OnDebugDrawSelected();
    if (Icon && Icon.GetID() != iconId)
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
    if (iconId != Guid::Empty && !Editor::IsPlayMode)
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
    if (Type == NetworkingType::Registered && !Despawning)
    {
        Networking::Instance->StopReplicating(this);
    }
    Actor::OnDisable();
}

void Entity::OnEnable()
{
    Actor::OnEnable();
#if USE_EDITOR
    if (Icon && !Editor::IsPlayMode)
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

    if (Type == NetworkingType::Registered)
    {
        Networking::Instance->StartReplicating(this);
    }
    else if (NetworkManager::IsClient() && Type == NetworkingType::HostOnly)
    {
        DeleteObject();
        return;
    }
}

const Dictionary<String, IComponent*> Entity::GetComponents()
{
    CacheComponents();
    Dictionary<String, IComponent*> result;
    for (const auto& entry : Components)
    {
        result[entry.Key.ToString()] = entry.Value;
    }
    return result;
}

ScriptingObjectReference<IComponent> Entity::GetComponent(const MClass* type)
{
    CacheComponents();
    IComponent* result = nullptr;
    Components.TryGet(type->GetFullName(), result);
    return result;
}

ScriptingObjectReference<IComponent> Entity::GetComponent(const ScriptingTypeHandle& type)
{
    CacheComponents();
    IComponent* result = nullptr;
    Components.TryGet(type.GetType().Fullname, result);
    return result;
}

Entity* Entity::FindEntity(Script* Script)
{
    return FindEntity(Script->GetActor());
}

Entity* Entity::FindEntity(Actor* Child)
{
    if (!Child)
    {
        return nullptr;
    }
    while (true)
    {
        Entity* entity = Cast<Entity>(Child);
        if (entity)
        {
            return entity;
        }
        Child = Child->GetParent();
        if (!Child)
        {
            return nullptr;
        }
    }
}
