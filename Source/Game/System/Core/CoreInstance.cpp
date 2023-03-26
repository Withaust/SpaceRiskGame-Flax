#include "CoreInstance.h"

CoreInstance* CoreInstance::_instance = nullptr;

CoreInstance::CoreInstance(const SpawnParams& params)
    : Script(params)
{
    _instance = this;
}

CoreInstance* CoreInstance::Instance()
{
    return _instance;
}

void CoreInstance::OnEnable()
{
    Level::SceneLoaded.Bind<CoreInstance, &CoreInstance::OnSceneLoaded>(this);
    Level::SceneLoaded.Bind<CoreInstance, &CoreInstance::OnSceneUnloaded>(this);
    LoadSystems();
}

void CoreInstance::OnDisable()
{
    for (int i = _systems_array.Count() - 1; i >= 0; i--)
    {
        _system_dict.Remove(_systems_array[i]->GetStaticType().Fullname);
        _systems_array[i]->OnDeinitialize();
        _systems_array.RemoveAtKeepOrder(i);
    }
    Level::SceneLoaded.Unbind<CoreInstance, &CoreInstance::OnSceneLoaded>(this);
    Level::SceneLoaded.Unbind<CoreInstance, &CoreInstance::OnSceneUnloaded>(this);
}

void CoreInstance::OnSceneLoaded(Scene* scene, const Guid& sceneId)
{
    for (int i = 0; i < _systems_array.Count(); ++i)
    {
        _systems_array[i]->OnSceneLoaded(scene);
    }
}

void CoreInstance::OnSceneUnloaded(Scene* scene, const Guid& sceneId)
{
    for (int i = 0; i < _systems_array.Count(); ++i)
    {
        _systems_array[i]->OnSceneUnloaded(scene);
    }
}

void CoreInstance::OnPlayerConnected(NetworkClient* client)
{
    for (int i = 0; i < _systems_array.Count(); ++i)
    {
        _systems_array[i]->OnPlayerConnected(client);
    }
}

void CoreInstance::OnPlayerDisconnected(NetworkClient* client)
{
    for (int i = 0; i < _systems_array.Count(); ++i)
    {
        _systems_array[i]->OnPlayerDisconnected(client);
    }
}

void CoreInstance::ReplicateSystems()
{
    for (int i = 0; i < _systems_array.Count(); ++i)
    {
        NetworkReplicator::AddObject(_systems_array[i]);
    }
}

ISystem* CoreInstance::Get(const MClass* type)
{
    return _system_dict[type->GetFullName()];
}

ISystem* CoreInstance::Get(const ScriptingTypeHandle& type)
{
    return _system_dict[type.GetType().Fullname];
}

#include <Game/System/Core/LaunchArgs.h>
#include <Game/System/Core/Steam.h>
#include <Game/System/Core/Analytics.h>
#include <Game/System/Core/Logger.h>
#include <Game/System/Core/InfoWare.h>
#include <Game/System/Core/Networking.h>
#include <Game/System/Core/LevelManager.h>
// CoreInitializer should always be used last
#include <Game/System/Core/CoreInitializer.h>

void CoreInstance::LoadSystems()
{
    Add<LaunchArgs>();
    Add<Steam>();
    Add<Analytics>();
    Add<Logger>();
    Add<InfoWare>();
    Add<Networking>();
    Add<LevelManager>();
    // CoreInitializer should always be used last
    Add<CoreInitializer>();
}
