#include "SceneRoot.h"

SceneRoot::SceneRoot(const SpawnParams& params)
    : Script(params)
{
}

void SceneRoot::OnAwake()
{
    Array<Actor*> scenes;
    Level::GetScenes(scenes);

    bool gotCore = false;
    for (const auto& scene : scenes)
    {
        if (scene->GetName() == TEXT("Core"))
        {
            gotCore = true;
            break;
        }
    }

    if (gotCore)
    {
        return;
    }

    LaunchArgs::Instance->Level = GetParent()->GetName();

    Level::UnloadAllScenes();
    Level::LoadScene(GameSettings::Get()->FirstScene);
}
