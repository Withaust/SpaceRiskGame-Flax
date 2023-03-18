#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Level.h>
#include <Engine/Core/Collections/Dictionary.h>
#include <Engine/Scripting/ManagedCLR/MClass.h>
#include <Engine/Level/Actor.h>
#include <Engine/Level/Scene/Scene.h>

#include <Game/System/GameSystem.h>

API_CLASS() class GAME_API GameInstance : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(GameInstance);
private:
    static GameInstance* _instance;

    Array<GameSystem*> _systems_array;
    Dictionary<StringAnsiView, GameSystem*> _system_dict;

public:

    API_PROPERTY() static GameInstance* GetInstance();

    void OnEnable() override;
    void OnDisable() override;

    void OnSceneLoaded(Scene* scene, const Guid& sceneId);
    void OnSceneUnloaded(Scene* scene, const Guid& sceneId);

    API_PROPERTY() FORCE_INLINE const Array<GameSystem*>& GetSystems() const
    {
        return _systems_array;
    }

    API_FUNCTION() GameSystem* Get(API_PARAM(Attributes = "TypeReference(typeof(GameSystem))") const MClass* type);
    GameSystem* Get(const ScriptingTypeHandle& type);

    template<typename T>
    FORCE_INLINE T* Get()
    {
        static_assert(std::is_base_of<GameSystem, T>::value, "T must inherit GameSystem to be used with Get()");
        return static_cast<T*>(Get(T::TypeInitializer));
    }

    template<typename T>
    void Add()
    {
        static_assert(std::is_base_of<GameSystem, T>::value, "T must inherit GameSystem to be used with Add()");
        T* NewGameSystem = GetActor()->FindScript<T>();
        if (NewGameSystem == nullptr)
        {
            Platform::Error(String("Failed add system ") + String(T::TypeInitializer.GetType().Fullname));
            Engine::RequestExit(1);
            return;
        }
        _systems_array.Add(NewGameSystem);
        _system_dict[T::TypeInitializer.GetType().Fullname] = NewGameSystem;
        NewGameSystem->OnInitialize();
    }

    void LoadSystems();
};

#define GET_SYSTEM(System) GameInstance::GetInstance()->Get<System>()
