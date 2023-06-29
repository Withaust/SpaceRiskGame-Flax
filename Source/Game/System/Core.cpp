#include "Core.h"

Core* Core::Instance = nullptr;
Array<ScriptingObjectReference<ISystem>> Core::_systemsArray;
Dictionary<StringAnsiView, ScriptingObjectReference<ISystem>> Core::_systemsDict;

Core::Core(const SpawnParams& params)
    : Actor(params)
{
    Instance = this;
    _systemsArray.Clear();
    _systemsDict.Clear();
}

void Core::OnEnable()
{
    Actor::OnEnable();
#if USE_EDITOR
    if (!Editor::IsPlayMode)
    {
        return;
    }
#endif
    LoadSystems();
}

void Core::OnDisable()
{
#if USE_EDITOR
    if (!Editor::IsPlayMode)
    {
        Actor::OnDisable();
        return;
    }
#endif
    for (int i = _systemsArray.Count() - 1; i >= 0; i--)
    {
        _systemsDict.Remove(_systemsArray[i]->GetStaticType().Fullname);
        _systemsArray[i]->OnDeinitialize();
        _systemsArray.RemoveAtKeepOrder(i);
    }
    Instance = nullptr;
    Actor::OnDisable();
}

void Core::ReplicateSystems()
{
    NetworkReplicator::AddObject(GetParent());
    NetworkReplicator::AddObject(this);

    for (int i = 0; i < _systemsArray.Count(); ++i)
    {
        const auto& target = _systemsArray[i];
        if (target->_replicate)
        {
            NetworkReplicator::AddObject(target->GetParent());
            NetworkReplicator::AddObject(target);
        }
    }
}

ScriptingObjectReference<ISystem> Core::Get(const MClass* type)
{
    ScriptingObjectReference<ISystem> result;
    if (!_systemsDict.TryGet(type->GetFullName(), result))
    {
        Platform::Error(String("Failed to get system ") + String(type->GetFullName()));
        Engine::RequestExit(1);
        return {};
    }
    return result;
}

ScriptingObjectReference<ISystem> Core::Get(const ScriptingTypeHandle& type)
{
    ScriptingObjectReference<ISystem> result;
    if (!_systemsDict.TryGet(type.GetType().Fullname, result))
    {
        Platform::Error(String("Failed to get system ") + String(type.GetType().Fullname));
        Engine::RequestExit(1);
        return {};
    }
    return result;
}

// Core systems
#include <Game/System/Core/LaunchArgs.h>
#include <Game/System/Core/Steam.h>
#include <Game/System/Core/Analytics.h>
#include <Game/System/Core/Logger.h>
#include <Game/System/Core/InfoWare.h>
#include <Game/System/Core/Networking/Networking.h>
#include <Game/System/Core/LevelManager.h>
// Game systems
#include <Game/System/Game/VisibilityCPU.h>
#include <Game/System/Game/VisibilityGPU.h>
#include <Game/System/Game/Player/PlayerRespawns.h>
#include <Game/System/Game/Player/PlayerManager.h>
#include <Game/System/Game/Chat.h>
// UI
#include <Game/System/Core/UI.h>

// CoreInitializer should always be used last
#include <Game/System/Core/CoreInitializer.h>

void Core::LoadSystems()
{
    // Core systems
    Add<LaunchArgs>();
    Add<Steam>();
    Add<Analytics>();
    Add<Logger>();
    Add<InfoWare>();
    Add<Networking>(true);
    Add<LevelManager>(true);
    // Game systems
    Add<VisibilityCPU>();
    Add<VisibilityGPU>();
    Add<PlayerRespawns>(true);
    Add<PlayerManager>(true);
    Add<Chat>(true);
    // UI
    Add<UI>();

    // CoreInitializer should always be used last
    Add<CoreInitializer>();
}
