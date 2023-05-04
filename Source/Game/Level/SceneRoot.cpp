#include "SceneRoot.h"

SceneRoot::SceneRoot(const SpawnParams& params)
    : Script(params)
{
}

void SceneRoot::OnAwake()
{
    Array<Actor*> scenes;
    Level::GetScenes(scenes);
    bool hasCore = false;
    for (int i = 0; i < scenes.Count(); ++i)
    {
        if (scenes[i]->GetName() == TEXT("Core"))
        {
            hasCore = true;
            return;
        }
    }
    if (Core::Instance() == nullptr || !hasCore)
    {
        Platform::Error(TEXT("You are not supposed to launch any scene besides \"Core\". Engine will now force-quit, since the game would have crashed anyway."));
        Engine::Exit(1);
        return;
    }
}
