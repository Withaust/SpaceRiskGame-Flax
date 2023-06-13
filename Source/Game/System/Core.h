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

// Singleton macro for Core
#define UIMPL_SINGLETON(T) T* T::Instance = nullptr;

// This allows us to static_assert upon T::Instance mising while in Add<T>
template <typename T>
class HasInstance
{
    typedef char YesType;
    typedef long NoType;
    template <typename U> static YesType Test(decltype(&U::Instance));
    template <typename U> static NoType  Test(...);
public:
    static constexpr bool value = sizeof(Test<T>(0)) == sizeof(YesType);
};

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

    static Array<SystemEntry> _systemsArray;
    static Dictionary<StringAnsiView, SystemEntry> _systemsDict;

public:

    API_FIELD() static Core* Instance;

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
        static_assert(HasInstance<T>::value, "T must return Instance pointer for singleton implementation");
        static_assert(std::is_base_of<ISystem, T>::value, "T must inherit ISystem to be used with Add()");
        T* newGameSystem = Instance->FindScript<T>();
        if (newGameSystem == nullptr)
        {
            Platform::Error(String("Failed add system ") + String(T::TypeInitializer.GetType().Fullname));
            Engine::RequestExit(1);
            return;
        }
        SystemEntry newEntry;
        T::Instance = newGameSystem;
        newEntry.Ptr = newGameSystem;
        newEntry.Replicated = replicate;
        _systemsArray.Add(newEntry);
        _systemsDict[T::TypeInitializer.GetType().Fullname] = newEntry;
        newEntry.Ptr->OnInitialize();
    }

    static void LoadSystems();
};
