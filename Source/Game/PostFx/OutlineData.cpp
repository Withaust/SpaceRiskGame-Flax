#include "OutlineData.h"

OutlineData::OutlineData(const SpawnParams& params)
    : Script(params)
{
}

void OutlineData::OnEnable()
{
    if (!_materialInstance)
    {
        _materialInstance = Material->CreateVirtualInstance();
        _materialInstance->AddReference();
    }
    _materialInstance->SetBaseMaterial(Material);
}

void OutlineData::OnDisable()
{
    if (_materialInstance)
    {
        _materialInstance->SetBaseMaterial(nullptr);
        _materialInstance->Params.Resize(0);
        _materialInstance->RemoveReference();
        _materialInstance = nullptr;
    }
}

bool OutlineData::CanRender()
{
    return false;
}

void OutlineData::Render(Outline* target, GPUContext* context, RenderContext& renderContext, GPUTexture* input, GPUTexture* output)
{

}