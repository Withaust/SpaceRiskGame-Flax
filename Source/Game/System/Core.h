#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Scripting/Scripting.h>
#include <Engine/Level/Level.h>
#include <Engine/Core/Collections/Dictionary.h>
#include <Engine/Scripting/ManagedCLR/MClass.h>
#include <Engine/Level/Actor.h>
#include <Engine/Level/Scene/Scene.h>
#include <Engine/Networking/NetworkReplicator.h>
#include <Engine/Engine/Engine.h>
#if USE_EDITOR
#include <Editor/Editor.h>
#include <Editor/Managed/ManagedEditor.h>
#endif

#include <Game/System/ISystem.h>

API_CLASS(Attributes = "ActorContextMenu(\"New/Other/Core\"), ActorToolbox(\"Core\")") 
class GAME_API Core : public Actor
{
    DECLARE_SCENE_OBJECT(Core);
public:
    struct SystemEntry
    {
        ISystem* Ptr;
        bool Replicated;
    };

private:

    static Core* _instance;

    static Array<SystemEntry> _systemsArray;
    static Dictionary<StringAnsiView, SystemEntry> _systemsDict;

public:

    API_PROPERTY() static Core* Instance();

    void OnEnable() override;
    void OnDisable() override;

#define DISPATCH_SYSTEM(name, ...) \
    for (int i = 0; i < _systemsArray.Count(); ++i) \
    { \
        _systemsArray[i].Ptr->name(__VA_ARGS__); \
    }

    void OnSceneLoaded(Scene* scene) { DISPATCH_SYSTEM(OnSceneLoaded, scene) }
    void OnSceneUnloaded(Scene* scene) { DISPATCH_SYSTEM(OnSceneUnloaded, scene) }
    void OnConnected() { DISPATCH_SYSTEM(OnConnected) }
    void OnDisconnected() { DISPATCH_SYSTEM(OnDisconnected) }
    void OnPlayerConnected(NetworkClient* client) { DISPATCH_SYSTEM(OnPlayerConnected, client) }
    void OnPlayerDisconnected(NetworkClient* client) { DISPATCH_SYSTEM(OnPlayerDisconnected, client) }

#undef DISPATCH_SYSTEM

    API_FUNCTION() void ReplicateSystems();

    API_FUNCTION() static ISystem* Get(API_PARAM(Attributes = "TypeReference(typeof(ISystem))") const MClass* type);
    static ISystem* Get(const ScriptingTypeHandle& type);

    template<typename T>
    FORCE_INLINE static T* Get()
    {
        static_assert(std::is_base_of<ISystem, T>::value, "T must inherit ISystem to be used with Get()");
        return static_cast<T*>(Get(T::TypeInitializer));
    }

    template<typename T>
    static void Add(bool replicate = false)
    {
        static_assert(std::is_base_of<ISystem, T>::value, "T must inherit ISystem to be used with Add()");
        T* newGameSystem = _instance->FindScript<T>();
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

    static void Add(const StringAnsiView& scriptName, bool replicate = false)
    {
        const MString scriptNameStd = scriptName.ToStringAnsi();
        const ScriptingTypeHandle scriptingType = Scripting::FindScriptingType(scriptNameStd);
        if (!scriptingType)
        {
            Platform::Error(String("Failed add system ") + String(scriptName));
            Engine::RequestExit(1);
            return;
        }
        MClass* mclass = scriptingType.GetType().ManagedClass;
        ISystem* newGameSystem = static_cast<ISystem*>(_instance->FindScript(mclass));
        if (newGameSystem == nullptr)
        {
            Platform::Error(String("Failed add system ") + String(scriptName));
            Engine::RequestExit(1);
            return;
        }
        SystemEntry newEntry;
        newEntry.Ptr = newGameSystem;
        newEntry.Replicated = replicate;
        _systemsArray.Add(newEntry);
        _systemsDict[scriptName] = newEntry;
        newEntry.Ptr->OnInitialize();
    }

    static void LoadSystems();
};
