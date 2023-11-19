#include "Pov.h"

Pov::Pov(const SpawnParams& params)
    : PostProcessEffect(params)
{
    Location = PostProcessEffectLocation::AfterForwardPass;
    UseSingleTarget = true;
}

void Pov::OnEnable()
{
    Data = PostFx::Instance->PovData;
}

void Pov::OnDisable()
{
    Data->ShowModels();
    Data = nullptr;
}

bool Pov::CanRender() const
{
    return PostProcessEffect::CanRender() && Data && Data->CanRender();
}

void Pov::Render(GPUContext* context, RenderContext& renderContext, GPUTexture* input, GPUTexture* output)
{
    if (Data)
    {
        Data->Render(this, context, renderContext, input, output);
    }
}
