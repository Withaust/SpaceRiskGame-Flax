#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Level.h>
#include <Engine/Core/Collections/Dictionary.h>
#include <Engine/Scripting/ManagedCLR/MClass.h>
#include <Engine/Level/Actor.h>
#include <Engine/Level/Scene/Scene.h>
#include <Engine/Networking/NetworkReplicator.h>

#include <Game/System/Core/ISystem.h>

API_CLASS() class GAME_API CoreInstance : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(CoreInstance);
private:
    static CoreInstance* _instance;

    Array<ISystem*> _systems_array;
    Dictionary<StringAnsiView, ISystem*> _system_dict;

public:

    API_PROPERTY() static CoreInstance* Instance();

    void OnEnable() override;
    void OnDisable() override;

    void OnSceneLoaded(Scene* scene, const Guid& sceneId);
    void OnSceneUnloaded(Scene* scene, const Guid& sceneId);

    void OnPlayerConnected(NetworkClient* client);
    void OnPlayerDisconnected(NetworkClient* client);

    void ReplicateSystems();

    API_PROPERTY() FORCE_INLINE const Array<ISystem*>& GetSystems() const
    {
        return _systems_array;
    }

    API_FUNCTION() ISystem* Get(API_PARAM(Attributes = "TypeReference(typeof(ISystem))") const MClass* type);
    ISystem* Get(const ScriptingTypeHandle& type);

    template<typename T>
    FORCE_INLINE T* Get()
    {
        static_assert(std::is_base_of<ISystem, T>::value, "T must inherit ISystem to be used with Get()");
        return static_cast<T*>(Get(T::TypeInitializer));
    }

    template<typename T>
    void Add()
    {
        static_assert(std::is_base_of<ISystem, T>::value, "T must inherit ISystem to be used with Add()");
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

#define USINGLETON(Type) \
public: \
    static Type* Get() { return CoreInstance::Instance()->Get<Type>(); } \
private: 