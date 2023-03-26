#include "SceneRoot.h"

SceneRoot::SceneRoot(const SpawnParams& params)
    : Script(params)
{

}

void SceneRoot::OnAwake()
{
    if (CoreInstance::Instance() == nullptr)
    {
        Platform::Error(TEXT("You are not supposed to launch any scene besides \"Core\". Engine will now force-quit, since the game would have crashed anyway."));
        Engine::Exit(1);
        return;
    }
}
