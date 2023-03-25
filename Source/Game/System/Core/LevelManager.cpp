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
    AssetInfo Info;
    Content::GetAssetInfo(String::Format(TEXT("Content/Levels/{0}.scene"), scene), Info);

    if (Level::FindScene(Info.ID) != nullptr)
    {
        return;
    }

    Level::ScenesLock.Lock();
    auto scenes = Level::Scenes;
    for (int32 i = 0; i < scenes.Count(); i++)
    {
        Scene* Target = scenes[i];
        if (Target->GetFilename() == TEXT("Core"))
        {
            continue;
        }
        Level::UnloadScene(Target);
    }
    Level::ScenesLock.Unlock();

    Level::LoadSceneAsync(Info.ID);
}