#include "LevelManager.h"

LevelManager::LevelManager(const SpawnParams& params)
    : ISystem(params)
{
}

void LevelManager::OnInitialize()
{
    Level::SceneLoaded.Bind<LevelManager, &LevelManager::OnSceneLoaded>(this);
    Level::SceneLoaded.Bind<LevelManager, &LevelManager::OnSceneUnloaded>(this);
}

void LevelManager::OnDeinitialize()
{
    Level::SceneLoaded.Unbind<LevelManager, &LevelManager::OnSceneLoaded>(this);
    Level::SceneLoaded.Unbind<LevelManager, &LevelManager::OnSceneUnloaded>(this);
}

void LevelManager::OnSceneLoaded(Scene* scene, const Guid& id)
{
    if (scene->GetName() == TEXT("Core"))
    {
        return;
    }
    CoreInstance::Instance()->OnSceneLoaded(scene);
}

void LevelManager::OnSceneUnloaded(Scene* scene, const Guid& id)
{
    if (scene->GetName() == TEXT("Core"))
    {
        return;
    }
    CoreInstance::Instance()->OnSceneUnloaded(scene);
}

void LevelManager::LoadLevel(String scene)
{
    AssetInfo info;
    Content::GetAssetInfo(String::Format(TEXT("Content/Levels/{0}.scene"), scene), info);

    if (Level::FindScene(info.ID) != nullptr)
    {
        return;
    }

    Level::ScenesLock.Lock();
    auto scenes = Level::Scenes;
    for (int32 i = 0; i < scenes.Count(); i++)
    {
        Scene* target = scenes[i];
        if (target->GetName() == TEXT("Core"))
        {
            continue;
        }
        Level::UnloadScene(target);
    }
    Level::ScenesLock.Unlock();
    Level::LoadSceneAsync(info.ID);
}
