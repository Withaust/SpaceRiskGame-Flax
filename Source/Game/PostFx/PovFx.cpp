#include "PovFx.h"

PovFx::PovFx(const SpawnParams& params)
    : PostProcessEffect(params)
{
    Location = PostProcessEffectLocation::AfterForwardPass;
    UseSingleTarget = true;
}

void PovFx::SetPlayer(Entity* player)
{
    _ourPlayer = player;
}

void PovFx::OnEnable()
{
    RefreshModels();

    // Create new rendering task to draw
    _outputTexture = GPUDevice::Instance->CreateTexture(TEXT("PlayerPOV"));
    _renderingTask = New<SceneRenderTask>();
    _renderingTask->Output = _outputTexture;
    _renderingTask->Buffers->SetUseAlpha(true);

    // Create PSO that will draw weapon over the scene (rendered weapon is alpha-masked)
    auto psoDesc = GPUPipelineState::Description::DefaultFullscreenTriangle;
    psoDesc.PS = GPUDevice::Instance->QuadShader->GetPS("PS_CopyLinear");
    psoDesc.BlendMode = BlendingMode::AlphaBlend;
    psoDesc.BlendMode.SrcBlend = BlendingMode::Blend::One;
    psoDesc.BlendMode.DestBlend = BlendingMode::Blend::InvSrcAlpha;
    psoDesc.BlendMode.SrcBlendAlpha = BlendingMode::Blend::One;
    psoDesc.BlendMode.DestBlendAlpha = BlendingMode::Blend::Zero;
    psoDesc.BlendMode.BlendOp = BlendingMode::Operation::Add;
    _compositeOutputPipeline = GPUPipelineState::New();
    _compositeOutputPipeline->Init(psoDesc);
}

void PovFx::RefreshModels()
{
    ShowModels();
    _models.Clear();
    Array<StaticModel*> models;
    EngineHelper::FindActors(_ourPlayer, models);

    for (const auto& model : models)
    {
        Array<Material*> result;

        const auto slots = model->GetMaterialSlots();
        for (int i = 0; i < slots.Length(); ++i)
        {
            result.Add((Material*)model->GetMaterial(i));
        }

        _models[model] = result;
    }
}

void PovFx::HideModels()
{
    if (!_ourPlayer || EnumHasAnyFlags(_ourPlayer->Flags, ObjectFlags::WasMarkedToDelete))
    {
        return;
    }

    for (const auto& model : _models)
    {
        for (int i = 0; i < model.Value.Count(); ++i)
        {
            model.Key->SetMaterial(i, (MaterialBase*)ShadowBlocker);
        }
        model.Key->SetForcedLOD(100);
    }
}

void PovFx::ShowModels()
{
    if (!_ourPlayer || EnumHasAnyFlags(_ourPlayer->Flags, ObjectFlags::WasMarkedToDelete))
    {
        return;
    }

    for (const auto& model : _models)
    {
        for (int i = 0; i < model.Value.Count(); ++i)
        {
            model.Key->SetMaterial(i, (MaterialBase*)model.Value[i]);
        }
        model.Key->SetForcedLOD(0);
    }
}

void PovFx::OnDisable()
{
    ShowModels();
    // Cleanup
    if (_compositeOutputPipeline)
    {
        _compositeOutputPipeline->ReleaseGPU();
        SAFE_DELETE_GPU_RESOURCE(_compositeOutputPipeline);
    }
    if (_outputTexture)
    {
        _outputTexture->ReleaseGPU();
        SAFE_DELETE_GPU_RESOURCE(_outputTexture);
    }
    if (_renderingTask)
    {
        SAFE_DELETE(_renderingTask);
    }
}

void PovFx::Render(GPUContext* context, RenderContext& renderContext, GPUTexture* input, GPUTexture* output)
{
    if (!_renderingTask)
    {
        return;
    }
#if USE_EDITOR
    auto p = ProfilerGPU::BeginEvent(TEXT("PlayerPOV"));
#endif
    auto width = input->Width();
    auto height = input->Height();

    // Initialize weapon rendering
    auto view = renderContext.View;
    view.Mode = ViewMode::NoPostFx; // Just render weapon with lighting, postfx are applied once for a whole game view
    view.RenderLayersMask = Layers; // Render selected layers
    view.Flags = RenderFlags; // Select visual features to use during rendering

    _renderingTask->View = view;

    // Setup rendering resolution
    if (!_outputTexture->IsAllocated())
    {
        auto outputDesc = GPUTextureDescription::New2D(width, height, _renderingTask->Buffers->GetOutputFormat());
        _outputTexture->Init(outputDesc);
    }
    _renderingTask->Resize(width, height);

    // Reuse main game viewport GI/GlobalSDF/etc when rendering weapon
    _renderingTask->Buffers->LinkedCustomBuffers = renderContext.Buffers;

    ShowModels();

    // Render nested scene with weapon-only
    Renderer::Render(_renderingTask);

    // Composite weapon over the scene view
    context->ResetRenderTarget();
    context->SetViewport((float)width, (float)height);
    context->SetRenderTarget(input->View());
    context->BindSR(0, _outputTexture->View());
    auto pipeline = _compositeOutputPipeline;
    context->SetState(pipeline);
    context->DrawFullscreenTriangle();

    HideModels();
#if USE_EDITOR
    ProfilerGPU::EndEvent(p);
#endif
}
