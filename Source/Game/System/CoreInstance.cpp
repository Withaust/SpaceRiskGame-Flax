#include "CoreInstance.h"

CoreInstance* CoreInstance::_Instance = nullptr;

CoreInstance::CoreInstance(const SpawnParams& params)
    //: Actor(SpawnParams(Guid(0x12345678, 0x99634f61, 0x84723632, 0x54c776af), params.Type)) // Override ID to be the same on all clients (a cross-device singleton) to keep network id stable
    : Actor(params)
{
    _Instance = this;
}

CoreInstance* CoreInstance::Instance()
{
    return _Instance;
}

void CoreInstance::OnEnable()
{
#if USE_EDITOR
    if (!Editor::IsPlayMode)
    {
        return;
    }
#endif
    Level::SceneLoaded.Bind<CoreInstance, &CoreInstance::OnSceneLoaded>(this);
    Level::SceneLoaded.Bind<CoreInstance, &CoreInstance::OnSceneUnloaded>(this);
    LoadSystems();
}

void CoreInstance::OnDisable()
{
#if USE_EDITOR
    if (!Editor::IsPlayMode)
    {
        return;
    }
#endif
    for (int i = _SystemsArray.Count() - 1; i >= 0; i--)
    {
        _SystemsDict.Remove(_SystemsArray[i].Ptr->GetStaticType().Fullname);
        _SystemsArray[i].Ptr->OnDeinitialize();
        _SystemsArray.RemoveAtKeepOrder(i);
    }
    Level::SceneLoaded.Unbind<CoreInstance, &CoreInstance::OnSceneLoaded>(this);
    Level::SceneLoaded.Unbind<CoreInstance, &CoreInstance::OnSceneUnloaded>(this);
}

void CoreInstance::OnSceneLoaded(Scene* scene, const Guid& sceneId)
{
    if (scene->GetName() == TEXT("Core"))
    {
        return;
    }
    for (int i = 0; i < _SystemsArray.Count(); ++i)
    {
        _SystemsArray[i].Ptr->OnSceneLoaded(scene);
    }
}

void CoreInstance::OnSceneUnloaded(Scene* scene, const Guid& sceneId)
{
    if (scene->GetName() == TEXT("Core"))
    {
        return;
    }
    for (int i = 0; i < _SystemsArray.Count(); ++i)
    {
        _SystemsArray[i].Ptr->OnSceneUnloaded(scene);
    }
}

void CoreInstance::OnPlayerConnected(NetworkClient* client)
{
    for (int i = 0; i < _SystemsArray.Count(); ++i)
    {
        _SystemsArray[i].Ptr->OnPlayerConnected(client);
    }
}

void CoreInstance::OnPlayerDisconnected(NetworkClient* client)
{
    for (int i = 0; i < _SystemsArray.Count(); ++i)
    {
        _SystemsArray[i].Ptr->OnPlayerDisconnected(client);
    }
}

void CoreInstance::ReplicateSystems()
{
    NetworkReplicator::AddObject(GetParent());
    NetworkReplicator::AddObject(this);
    for (int i = 0; i < _SystemsArray.Count(); ++i)
    {
        const auto& Target = _SystemsArray[i];
        if (Target.Replicated)
        {
            NetworkReplicator::AddObject(Target.Ptr);
        }
    }
}

ISystem* CoreInstance::Get(const MClass* type)
{
    SystemEntry* Entry = _SystemsDict.TryGet(type->GetFullName());
    if (Entry == nullptr)
    {
        return nullptr;
    }
    return Entry->Ptr;
}

ISystem* CoreInstance::Get(const ScriptingTypeHandle& type)
{
    SystemEntry* Entry = _SystemsDict.TryGet(type.GetType().Fullname);
    if (Entry == nullptr)
    {
        return nullptr;
    }
    return Entry->Ptr;
}

// Core systems
#include <Game/System/Core/LaunchArgs.h>
#include <Game/System/Core/Steam.h>
#include <Game/System/Core/Analytics.h>
#include <Game/System/Core/Logger.h>
#include <Game/System/Core/InfoWare.h>
#include <Game/System/Core/Networking.h>
#include <Game/System/Core/LevelManager.h>
// CoreInitializer should always be used last in core
#include <Game/System/Core/CoreInitializer.h>
// Game systems
#include <Game/System/Game/VisibilityCPU.h>
#include <Game/System/Game/VisibilityGPU.h>
#include <Game/System/Game/PlayerRespawn.h>
// UI systems
#include <Game/UI/UIRoot.h>

void CoreInstance::LoadSystems()
{
    // Core systems
    Add<LaunchArgs>();
    Add<Steam>();
    Add<Analytics>();
    Add<Logger>();
    Add<InfoWare>();
    Add<Networking>();
    Add<LevelManager>();
    // CoreInitializer should always be used last in core
    Add<CoreInitializer>();
    // Game systems
    Add<VisibilityCPU>();
    Add<VisibilityGPU>();
    Add<PlayerRespawn>();
    // UI systems
    Add<UIRoot>(false);
}
