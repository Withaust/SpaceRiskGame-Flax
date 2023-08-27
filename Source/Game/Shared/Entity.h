#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actor.h>
#if USE_EDITOR
#include <Editor/Editor.h>
#include <Engine/Content/AssetReference.h>
#include <Engine/Content/Assets/Texture.h>
#include <Engine/Scripting/BinaryModule.h>
#include <Engine/Scripting/ManagedCLR/MClass.h>
#include <Engine/Scripting/ManagedCLR/MMethod.h>
#include <Engine/Scripting/Types.h>
#include <Editor/Utilities/ViewportIconsRenderer.h>
#endif

#include <Game/Shared/Components/IComponent.h>
#include <Game/System/Core/Logger.h>

API_CLASS(Attributes = "ActorContextMenu(\"New/Entity\"), ActorToolbox(\"Other\")")
class GAME_API Entity : public Actor
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Entity);
    friend class IComponent;
    friend class Networking;

#if USE_EDITOR
private:
    Guid iconId;
    static bool proxyAdded;
    void CheckProxy();
#endif
    bool Despawning = false;
    bool GotComponents = false;
    Dictionary<StringAnsiView, IComponent*> Components;
    void CacheComponents();
public:

    API_ENUM() enum class NetworkingType
    {
        None,
        HostOnly,
        Registered,
    };

    API_FIELD() NetworkingType Type = NetworkingType::None;
#if USE_EDITOR
    API_FIELD() AssetReference<Texture> Icon;
    void OnDebugDrawSelected() override;
#endif
    void OnDisable() override;
    void OnEnable() override;

    API_FUNCTION() const Dictionary<String, IComponent*> GetComponents();

    API_FUNCTION() ScriptingObjectReference<IComponent> GetComponent(API_PARAM(Attributes = "TypeReference(typeof(IComponent))") const MClass* type);
    ScriptingObjectReference<IComponent> GetComponent(const ScriptingTypeHandle& type);

    template<typename T>
    FORCE_INLINE T* GetComponent()
    {
        static_assert(std::is_base_of<IComponent, T>::value, "T must inherit IComponent to be used with GetComponent()");
        return Cast<T>(GetComponent(T::TypeInitializer));
    }

    API_FUNCTION() static Entity* FindEntity(Actor* Child)
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
};

typedef Entity::NetworkingType EntNetType;
