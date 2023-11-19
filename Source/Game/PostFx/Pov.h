#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Graphics/PostProcessEffect.h>

#include <Game/Shared/Defines.h>
#include <Game/PostFx/PostFx.h>
#include <Game/PostFx/PovData.h>

API_CLASS() class GAME_API Pov : public PostProcessEffect
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Pov);
    friend class PovData;

private:

    PovData* Data = nullptr;

public:

    void OnEnable() override;
    void OnDisable() override;
    bool CanRender() const override;
    void Render(GPUContext* context, RenderContext& renderContext, GPUTexture* input, GPUTexture* output) override;
};
