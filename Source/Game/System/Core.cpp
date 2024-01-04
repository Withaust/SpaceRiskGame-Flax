#include "Core.h"
#include <Game/System/Core/Networking.h>

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
        if (target->Type != INetworkedObject::NetworkingType::None)
        {
            NetworkReplicator::AddObject(target->GetParent());
            NetworkReplicator::AddObject(target);
        }
        if (target->Type == INetworkedObject::NetworkingType::Replicated)
        {
            Networking::Instance->AddReplicatedSystem(Cast<ScriptingObject>(target));
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
#include <Game/System/Core/SyncInfo.h>
#include <Game/System/Core/LevelManager.h>
// Game systems
#include <Game/Save/Saver.h>
#include <Game/System/Game/VisibilityCPU.h>
#include <Game/System/Game/VisibilityGPU.h>
#include <Game/System/Game/Map.h>
#include <Game/System/Game/Player/PlayerRespawns.h>
#include <Game/System/Game/Player/PlayerManager.h>
#include <Game/System/Game/Chat.h>
#include <Game/System/Game/KillFeed.h>
#include <Game/System/Game/RobotRespawns.h>
// PostFx
#include <Game/PostFx/PostFx.h>
// UI
#include <Game/UI/UI.h>

// CoreInitializer should always be used last
#include <Game/System/Core/CoreInitializer.h>

void Core::LoadSystems()
{
    // Core systems
    Add<LaunchArgs>();
    Add<Steam>();
    Add<Analytics>();
    Add<Logger>();
    Add<Networking>();
    Add<SyncInfo>(INetworkedObject::NetworkingType::Registered);
    Add<LevelManager>(INetworkedObject::NetworkingType::Registered);
    // Game systems
    Add<Saver>();
    Add<VisibilityCPU>();
    Add<VisibilityGPU>();
    Add<Map>();
    Add<PlayerRespawns>(INetworkedObject::NetworkingType::Registered);
    Add<PlayerManager>(INetworkedObject::NetworkingType::Registered);
    Add<Chat>(INetworkedObject::NetworkingType::Registered);
    Add<KillFeed>(INetworkedObject::NetworkingType::Registered);
    Add<RobotRespawns>(INetworkedObject::NetworkingType::Registered);

    // PostFx
    Add<PostFx>();
    // UI
    Add<UI>();

    // CoreInitializer should always be used last
    Add<CoreInitializer>();
}
