#include "CoreInstance.h"

CoreInstance* CoreInstance::_instance = nullptr;

CoreInstance::CoreInstance(const SpawnParams& params)
    : Actor(params)
{
    _instance = this;
}

CoreInstance* CoreInstance::Instance()
{
    return _instance;
}

void CoreInstance::OnEnable()
{
    Level::ReversedUnload = true;
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
    for (int i = _systemsArray.Count() - 1; i >= 0; i--)
    {
        _systemsDict.Remove(_systemsArray[i].Ptr->GetStaticType().Fullname);
        _systemsArray[i].Ptr->OnDeinitialize();
        _systemsArray.RemoveAtKeepOrder(i);
    }
    Level::SceneLoaded.Unbind<CoreInstance, &CoreInstance::OnSceneLoaded>(this);
    Level::SceneLoaded.Unbind<CoreInstance, &CoreInstance::OnSceneUnloaded>(this);
}

void CoreInstance::OnSceneLoaded(Scene* scene, const Guid& id)
{
    if (scene->GetName() == TEXT("Core"))
    {
        return;
    }
    for (int i = 0; i < _systemsArray.Count(); ++i)
    {
        _systemsArray[i].Ptr->OnSceneLoaded(scene);
    }
}

void CoreInstance::OnSceneUnloaded(Scene* scene, const Guid& id)
{
    if (scene->GetName() == TEXT("Core"))
    {
        return;
    }
    for (int i = 0; i < _systemsArray.Count(); ++i)
    {
        _systemsArray[i].Ptr->OnSceneUnloaded(scene);
    }
}

void CoreInstance::OnPlayerConnected(NetworkClient* client)
{
    for (int i = 0; i < _systemsArray.Count(); ++i)
    {
        _systemsArray[i].Ptr->OnPlayerConnected(client);
    }
}

void CoreInstance::OnPlayerDisconnected(NetworkClient* client)
{
    for (int i = 0; i < _systemsArray.Count(); ++i)
    {
        _systemsArray[i].Ptr->OnPlayerDisconnected(client);
    }
}

void CoreInstance::ReplicateSystems()
{
    NetworkReplicator::AddObject(GetParent());
    NetworkReplicator::AddObject(this);

    for (int i = 0; i < _systemsArray.Count(); ++i)
    {
        const auto& Target = _systemsArray[i];
        if (Target.Replicated)
        {
            NetworkReplicator::AddObject(Target.Ptr->GetParent());
            NetworkReplicator::AddObject(Target.Ptr);
        }
    }
}

ISystem* CoreInstance::Get(const MClass* type)
{
    SystemEntry* Entry = _systemsDict.TryGet(type->GetFullName());
    if (Entry == nullptr)
    {
        return nullptr;
    }
    return Entry->Ptr;
}

ISystem* CoreInstance::Get(const ScriptingTypeHandle& type)
{
    SystemEntry* Entry = _systemsDict.TryGet(type.GetType().Fullname);
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
    Add<PlayerRespawn>(true);
    // UI systems
    Add<UIRoot>();
}
