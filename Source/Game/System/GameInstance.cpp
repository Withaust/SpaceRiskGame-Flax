#include "GameInstance.h"

GameInstance* GameInstance::_instance = nullptr;

GameInstance::GameInstance(const SpawnParams& params)
    : Script(params)
{
    _instance = this;
}

GameInstance* GameInstance::GetInstance()
{
    return _instance;
}

void GameInstance::OnEnable()
{
    Level::SceneLoaded.Bind<GameInstance, &GameInstance::OnSceneLoaded>(this);
    LoadSystems();
}

void GameInstance::OnDisable()
{
    Level::SceneLoaded.Unbind<GameInstance, &GameInstance::OnSceneLoaded>(this);
    for (int i = _systems_array.Count() - 1; i >= 0; i--)
    {
        _system_dict.Remove(_systems_array[i]->GetStaticType().Fullname);
        _systems_array[i]->OnDeinitialize();
        _systems_array.RemoveAtKeepOrder(i);
    }
}

void GameInstance::OnSceneLoaded(Scene* scene, const Guid& sceneId)
{
    for (int i = 0; i < _systems_array.Count(); ++i)
    {
        _systems_array[i]->OnSceneLoaded(scene);
    }
}

void GameInstance::OnSceneUnloaded(Scene* scene, const Guid& sceneId)
{
    for (int i = 0; i < _systems_array.Count(); ++i)
    {
        _systems_array[i]->OnSceneUnloaded(scene);
    }
}

GameSystem* GameInstance::Get(const MClass* type)
{
    return _system_dict[type->GetFullName()];
}

GameSystem* GameInstance::Get(const ScriptingTypeHandle& type)
{
    return _system_dict[type.GetType().Fullname];
}

#include <Game/System/LaunchArgs.h>
#include <Game/System/Steam.h>
#include <Game/System/GameAnalytics.h>
#include <Game/System/InfoWare.h>
#include <Game/System/Logger.h>

void GameInstance::LoadSystems()
{
    Add<LaunchArgs>();
    Add<Steam>();
    Add<GameAnalytics>();
    Add<InfoWare>();
    Add<Logger>();
}
