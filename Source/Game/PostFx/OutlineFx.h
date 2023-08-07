#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Graphics/PostProcessEffect.h>
#include <Engine/Content/Assets/MaterialInstance.h>
#include <Engine/Graphics/GPUResource.h>

API_CLASS() class GAME_API OutlineFx : public PostProcessEffect
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(OutlineFx);

private:

    MaterialInstance* _materialInstance = nullptr;
    Array<Actor*> _actors;
    Color _color = Color::Red;

public:

    API_FIELD() AssetReference<MaterialBase> Material;

    void OnEnable() override
    {
        if (!_materialInstance)
        {
            _materialInstance = Material->CreateVirtualInstance();
            _materialInstance->AddReference();
        }
        _materialInstance->SetBaseMaterial(Material);
    }

    void OnDisable() override
    {
        if (_materialInstance)
        {
            _materialInstance->SetBaseMaterial(nullptr);
            _materialInstance->Params.Resize(0);
            _materialInstance->RemoveReference();
            _materialInstance = nullptr;
        }
    }

    bool CanRender() const override
    {
        return PostProcessEffect::CanRender() && _materialInstance && _actors.Count() > 0;
    }

    void Render(GPUContext* context, RenderContext& renderContext, GPUTexture* input, GPUTexture* output) override
    {

    }
};
