#include "LevelManager.h"

LevelManager::LevelManager(const SpawnParams& params)
    : ISystem(params)
{
}

void LevelManager::OnInitialize()
{
}

void LevelManager::OnDeinitialize()
{
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
