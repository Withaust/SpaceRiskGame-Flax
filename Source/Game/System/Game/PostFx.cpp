#include "PostFx.h"

UIMPL_SINGLETON(PostFx);

PostFx::PostFx(const SpawnParams& params)
    : ISystem(params)
{
}

void PostFx::Enable(Entity* LocalPawn)
{
    PovFx->SetPlayer(LocalPawn);

    for (const auto& script : GetActor()->Scripts)
    {
        script->SetEnabled(true);
    }

    MainRenderTask::Instance->AddCustomPostFx(PovFx);
    MainRenderTask::Instance->AddCustomPostFx(OutlineFx);
}

void PostFx::Disable()
{
    MainRenderTask::Instance->RemoveCustomPostFx(PovFx);
    MainRenderTask::Instance->RemoveCustomPostFx(OutlineFx);
}

void PostFx::OnDeinitialize()
{
    Disable();
}