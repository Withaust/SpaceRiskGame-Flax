#pragma once

#include <Engine/Scripting/Script.h>
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
#include <Game/PostFx/Pov.h>
#include <Game/System/Game/Player/PlayerManager.h>

API_CLASS() class GAME_API PovData : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PovData);
    friend class Pov;

private:

    SceneRenderTask* _renderingTask = nullptr;
    GPUTexture* _outputTexture = nullptr;
    GPUPipelineState* _compositeOutputPipeline = nullptr;
    ScriptingObjectReference<Entity> _ourPlayer;
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

    void OnEnable() override;
    void OnDisable() override;
    bool CanRender();
    void Render(Pov* target, GPUContext* context, RenderContext& renderContext, GPUTexture* input, GPUTexture* output);

    API_FUNCTION() void RefreshModels();
};
