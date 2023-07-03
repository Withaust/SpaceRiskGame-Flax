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

private:

    static Array<ScriptingObjectReference<ISystem>> _systemsArray;
    static Dictionary<StringAnsiView, ScriptingObjectReference<ISystem>> _systemsDict;

public:

    API_FIELD() static Core* Instance;

    void OnEnable() override;
    void OnDisable() override;

#define DISPATCH_SYSTEM(name, ...) \
    for (int i = 0; i < _systemsArray.Count(); ++i) \
    { \
        _systemsArray[i]->name(__VA_ARGS__); \
    }

    void OnSceneLoaded(Scene* scene) { DISPATCH_SYSTEM(OnSceneLoaded, scene) }
    void OnSceneUnloaded(Scene* scene) { DISPATCH_SYSTEM(OnSceneUnloaded, scene) }
    void OnConnected() { DISPATCH_SYSTEM(OnConnected) }
    void OnDisconnected() { DISPATCH_SYSTEM(OnDisconnected) }
    void OnPlayerConnected(NetworkClient* client) { DISPATCH_SYSTEM(OnPlayerConnected, client) }
    void OnPlayerDisconnected(NetworkClient* client) { DISPATCH_SYSTEM(OnPlayerDisconnected, client) }

#undef DISPATCH_SYSTEM

    API_FUNCTION() void ReplicateSystems();

    API_FUNCTION() static ScriptingObjectReference<ISystem> Get(API_PARAM(Attributes = "TypeReference(typeof(ISystem))") const MClass* type);
    static ScriptingObjectReference<ISystem> Get(const ScriptingTypeHandle& type);

    template<typename T>
    FORCE_INLINE static ScriptingObjectReference<T> Get()
    {
        static_assert(std::is_base_of<ISystem, T>::value, "T must inherit System to be used with Get()");
        return Cast<T>(Get(T::TypeInitializer));
    }

    template<typename T>
    static void Add(bool replicate = false, bool spawnOnly = false)
    {
        static_assert(HasInstance<T>::value, "T must return Instance pointer for singleton implementation");
        static_assert(std::is_base_of<ISystem, T>::value, "T must inherit System to be used with Add()");
        ScriptingObjectReference<T> targetScript = Instance->FindScript<T>();
        ScriptingObjectReference<ISystem> targetSystem = Cast<T>(targetScript);
        if (!targetScript || !targetSystem)
        {
            Platform::Error(String("Failed add system ") + String(T::TypeInitializer.GetType().Fullname));
            Engine::RequestExit(1);
            return;
        }
        T::Instance = targetScript.Get();
        if (replicate)
        {
            targetSystem->Networked = INetworkedObject::NetworkedType::Continuous;
        }
        if (spawnOnly)
        {
            targetSystem->Networked = INetworkedObject::NetworkedType::SpawnOnly;
        }
        _systemsArray.Add(targetSystem);
        _systemsDict[T::TypeInitializer.GetType().Fullname] = targetSystem;
        targetSystem->OnInitialize();
    }

    static void LoadSystems();
};
