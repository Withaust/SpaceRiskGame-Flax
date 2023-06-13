#include "Core.h"

Core* Core::Instance = nullptr;
Array<Core::SystemEntry> Core::_systemsArray;
Dictionary<StringAnsiView, Core::SystemEntry> Core::_systemsDict;

Core::Core(const SpawnParams& params)
    : Actor(params)
{
    Instance = this;
    _systemsArray.Clear();
    _systemsDict.Clear();
}

void Core::OnEnable()
{
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
        return;
    }
#endif
    for (int i = _systemsArray.Count() - 1; i >= 0; i--)
    {
        _systemsDict.Remove(_systemsArray[i].Ptr->GetStaticType().Fullname);
        _systemsArray[i].Ptr->OnDeinitialize();
        _systemsArray.RemoveAtKeepOrder(i);
    }
    Instance = nullptr;
}

void Core::ReplicateSystems()
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

ISystem* Core::Get(const MClass* type)
{
    SystemEntry* Entry = _systemsDict.TryGet(type->GetFullName());
    if (Entry == nullptr)
    {
        Platform::Error(String("Failed to get system ") + String(type->GetFullName()));
        Engine::RequestExit(1);
        return nullptr;
    }
    return Entry->Ptr;
}

ISystem* Core::Get(const ScriptingTypeHandle& type)
{
    SystemEntry* Entry = _systemsDict.TryGet(type.GetType().Fullname);
    if (Entry == nullptr)
    {
        Platform::Error(String("Failed to get system ") + String(type.GetType().Fullname));
        Engine::RequestExit(1);
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
// Game systems
#include <Game/System/Game/VisibilityCPU.h>
#include <Game/System/Game/VisibilityGPU.h>
#include <Game/System/Game/PlayerRespawns.h>
#include <Game/System/Game/PlayerManager.h>
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
    Add<Networking>();
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
