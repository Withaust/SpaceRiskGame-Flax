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
    DECLARE_SCENE_OBJECT(CoreInstance);
public:
    struct SystemEntry
    {
        ISystem* Ptr;
        bool Replicated;
    };

private:

    static CoreInstance* _instance;

    Array<SystemEntry> _systemsArray;
    Dictionary<StringAnsiView, SystemEntry> _systemsDict;

public:

    API_PROPERTY() static CoreInstance* Instance();

    void OnEnable() override;
    void OnDisable() override;

    void OnSceneLoaded(Scene* scene, const Guid& id);
    void OnSceneUnloaded(Scene* scene, const Guid& id);

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
    void Add(bool replicate = false)
    {
        static_assert(std::is_base_of<ISystem, T>::value, "T must inherit ISystem to be used with Add()");
        T* newGameSystem = FindScript<T>();
        if (newGameSystem == nullptr)
        {
            Platform::Error(String("Failed add system ") + String(T::TypeInitializer.GetType().Fullname));
            Engine::RequestExit(1);
            return;
        }
        SystemEntry newEntry;
        newEntry.Ptr = newGameSystem;
        newEntry.Replicated = replicate;
        _systemsArray.Add(newEntry);
        _systemsDict[T::TypeInitializer.GetType().Fullname] = newEntry;
        newEntry.Ptr->OnInitialize();
    }

    void LoadSystems();
};
