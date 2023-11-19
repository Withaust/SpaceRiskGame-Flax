#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Graphics/PostProcessEffect.h>
#include <Engine/Content/Assets/MaterialInstance.h>
#include <Engine/Graphics/GPUResource.h>

#include <Game/Shared/Defines.h>
#include <Game/PostFx/PostFx.h>
#include <Game/PostFx/OutlineData.h>

API_CLASS() class GAME_API Outline : public PostProcessEffect
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Outline);
    friend class OutlineData;

private:

    OutlineData* Data = nullptr;

public:

    void OnEnable() override;
    void OnDisable() override;
    bool CanRender() const override;
    void Render(GPUContext* context, RenderContext& renderContext, GPUTexture* input, GPUTexture* output) override;
};
