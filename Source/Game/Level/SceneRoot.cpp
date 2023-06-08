#include "SceneRoot.h"

String SceneRoot::EditorScene;

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

    EditorScene = GetParent()->GetName();

    Level::UnloadAllScenes();
    Level::LoadScene(GameSettings::Get()->FirstScene);
}
