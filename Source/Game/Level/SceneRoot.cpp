#include "SceneRoot.h"

AssetReference<JsonAsset> SceneRoot::EditorLaunch = nullptr;

SceneRoot::SceneRoot(const SpawnParams& params)
    : Script(params)
{
}

void SceneRoot::OnAwake()
{
    Array<Actor*> scenes;
    Level::GetScenes(scenes);

    if (scenes.Count() == 1 && scenes[0]->GetName() == TEXT("Core"))
    {
        return;
    }

    const auto gameSettings = GameSettings::Get();
    if (gameSettings)
    {
        Guid assetId = Guid::Empty;
        gameSettings->CustomSettings.TryGet(TEXT("DebugArgs"), assetId);
        const auto asset = Content::Load<JsonAsset>(assetId);
        if (asset)
        {
            EditorLaunch = asset;
        }
    }
    EditorLaunch->GetInstance<DebugArgs>()->LaunchScene = GetParent()->GetName();

    Level::UnloadAllScenes();
    Level::LoadScene(GameSettings::Get()->FirstScene);
}
