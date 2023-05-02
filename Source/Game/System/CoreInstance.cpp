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
// Game systems
#include <Game/System/Game/VisibilityCPU.h>
#include <Game/System/Game/VisibilityGPU.h>
#include <Game/System/Game/PlayerRespawn.h>
#include <Game/System/Game/Chat.h>
// UI systems
#include <Game/UI/UIRoot.h>

// CoreInitializer should always be used last
#include <Game/System/Core/CoreInitializer.h>

void CoreInstance::LoadSystems()
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
    Add<PlayerRespawn>(true);
    Add<Chat>(true);
    // UI systems
    Add<UIRoot>();

    // CoreInitializer should always be used last
    Add<CoreInitializer>();
}
