#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Graphics/PostProcessEffect.h>
#include <Engine/Content/Assets/MaterialInstance.h>
#include <Engine/Graphics/GPUResource.h>

#include <Game/Shared/Defines.h>
#include <Game/PostFx/Outline.h>

API_CLASS() class GAME_API OutlineData : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(OutlineData);
    friend class Outline;

private:

    MaterialInstance* _materialInstance = nullptr;
    Array<Actor*> _actors;
    Color _color = Color::Red;

public:

    API_FIELD() AssetReference<MaterialBase> Material;

    void OutlineData::OnEnable() override;
    void OutlineData::OnDisable() override;
    bool OutlineData::CanRender();
    void OutlineData::Render(Outline* target, GPUContext* context, RenderContext& renderContext, GPUTexture* input, GPUTexture* output);
};
