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
#include <Editor/Utilities/ViewportIconsRenderer.h>
#endif

#include <Game/Shared/IComponent.h>
#include <Game/System/Core/Logger.h>

API_CLASS(Attributes = "ActorContextMenu(\"New/Entity\"), ActorToolbox(\"Other\")")
class GAME_API Entity : public Actor
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Entity);
    friend class IComponent;

#if USE_EDITOR
private:
    Guid iconId;
    static bool proxyAdded;
    void CheckProxy();
#endif
    bool GotComponents = false;
    Dictionary<ScriptingTypeHandle, IComponent*> Components;
    void CacheComponents();
public:
#if USE_EDITOR
    API_FIELD() AssetReference<Texture> Icon;
    void OnDebugDrawSelected() override;
    void OnDisable() override;
#endif
    void OnEnable() override;

    template<class T>
    T* GetComponent()
    {
        static_assert(std::is_base_of<IComponent, T>::value, "T must inherit IComponent to be used with GetComponent()");
        if (!GotComponents)
        {
            CacheComponents();
            GotComponents = true;
        }
        return Cast<T>(Components[T::GetStaticType().GetHandle()]);
    }

    static Entity* FindEntity(Actor* Child)
    {
        while (true)
        {
            Entity* entity = Cast<Entity>(Child);
            if (entity)
            {
                return entity;
            }
            Child = Child->GetParent();
        }
    }
};
