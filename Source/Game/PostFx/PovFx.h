#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Graphics/PostProcessEffect.h>
#include <Engine/Graphics/RenderTask.h>
#include <Engine/Graphics/GPUPipelineState.h>
#include <Engine/Graphics/GPUDevice.h>
#include <Engine/Graphics/RenderBuffers.h>
#include <Engine/Graphics/GPUPipelineState.h>
#include <Engine/Graphics/Shaders/GPUShader.h>
#include <Engine/Graphics/Shaders/Cache/ShaderAssetBase.h>
#include <Engine/Profiler/ProfilerGPU.h>
#include <Engine/Renderer/Renderer.h>
#include <Engine/Content/Assets/Material.h>
#include <Engine/Graphics/GPUContext.h>
#include <Engine/Level/Actors/Camera.h>
#include <Engine/Level/Actors/StaticModel.h>

#include <Game/Shared/Defines.h>

API_CLASS() class GAME_API PovFx : public PostProcessEffect
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PovFx);
private:

    SceneRenderTask* _renderingTask = nullptr;
    GPUTexture* _outputTexture = nullptr;
    GPUPipelineState* _compositeOutputPipeline = nullptr;
    Entity* _ourPlayer = nullptr;
    Dictionary<StaticModel*, Array<Material*>> _models;

    void HideModels();
    void ShowModels();

public:

    API_FIELD() AssetReference<Material> ShadowBlocker;
    API_FIELD() LayersMask Layers;
    API_FIELD() ViewFlags RenderFlags = ViewFlags::DirectionalLights |
        ViewFlags::SkyLights | ViewFlags::SpotLights |
        ViewFlags::PointLights | ViewFlags::SpecularLight |
        ViewFlags::Shadows | ViewFlags::ContactShadows |
        ViewFlags::Fog | ViewFlags::Reflections |
        ViewFlags::GI;

    void SetPlayer(Entity* player);
    void OnEnable() override;
    void OnDisable() override;
    void Render(GPUContext* context, RenderContext& renderContext, GPUTexture* input, GPUTexture* output) override;

    API_FUNCTION() void RefreshModels();
};
