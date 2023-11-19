#include "Outline.h"

Outline::Outline(const SpawnParams& params)
    : PostProcessEffect(params)
{
}

void Outline::OnEnable()
{
    Data = PostFx::Instance->OutlineData;
}

void Outline::OnDisable()
{
    Data = nullptr;
}

bool Outline::CanRender() const
{
    return PostProcessEffect::CanRender() && Data && Data->CanRender();
}

void Outline::Render(GPUContext* context, RenderContext& renderContext, GPUTexture* input, GPUTexture* output)
{
    if (Data)
    {
        Data->Render(this, context, renderContext, input, output);
    }
}
