#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Level.h>
#include <Engine/Core/Collections/Dictionary.h>
#include <Engine/Scripting/ManagedCLR/MClass.h>
#include <Engine/Level/Actor.h>
#include <Engine/Level/Scene/Scene.h>
#include <Engine/Networking/NetworkReplicator.h>
#if USE_EDITOR
#include <Editor/Editor.h>
#include <Editor/Managed/ManagedEditor.h>
#endif

#include <Game/System/Core/ISystem.h>

API_CLASS() class GAME_API CoreInstance : public Actor
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(CoreInstance);
public:
    struct SystemEntry
    {
        ISystem* Ptr;
        bool Replicated;
    };

private:
    static CoreInstance* _Instance;

    Array<SystemEntry> _SystemsArray;
    Dictionary<StringAnsiView, SystemEntry> _SystemsDict;

public:

    API_PROPERTY() static CoreInstance* Instance();

    void OnEnable() override;
    void OnDisable() override;

    void OnSceneLoaded(Scene* scene, const Guid& sceneId);
    void OnSceneUnloaded(Scene* scene, const Guid& sceneId);

    void OnPlayerConnected(NetworkClient* client);
    void OnPlayerDisconnected(NetworkClient* client);

    void ReplicateSystems();

    API_FUNCTION() ISystem* Get(API_PARAM(Attributes = "TypeReference(typeof(ISystem))") const MClass* type);
    ISystem* Get(const ScriptingTypeHandle& type);

    template<typename T>
    FORCE_INLINE T* Get()
    {
        static_assert(std::is_base_of<ISystem, T>::value, "T must inherit ISystem to be used with Get()");
        return static_cast<T*>(Get(T::TypeInitializer));
    }

    template<typename T>
    void Add(bool Replicate = true)
    {
        static_assert(std::is_base_of<ISystem, T>::value, "T must inherit ISystem to be used with Add()");
        T* NewGameSystem = FindScript<T>();
        if (NewGameSystem == nullptr)
        {
            Platform::Error(String("Failed add system ") + String(T::TypeInitializer.GetType().Fullname));
            Engine::RequestExit(1);
            return;
        }
        SystemEntry NewEntry;
        NewEntry.Ptr = NewGameSystem;
        NewEntry.Replicated = Replicate;
        _SystemsArray.Add(NewEntry);
        _SystemsDict[T::TypeInitializer.GetType().Fullname] = NewEntry;
        NewEntry.Ptr->OnInitialize();
    }

    void LoadSystems();
};
