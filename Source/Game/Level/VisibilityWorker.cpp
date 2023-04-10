#include "VisibilityWorker.h"

VisibilityWorker::VisibilityWorker(const SpawnParams& params)
    : Script(params)
{
    _tickUpdate = true;
}

bool VisibilityWorker::IsFinished()
{
    return _Finished;
}

void VisibilityWorker::Stop()
{
    _Stop = true;
}

void VisibilityWorker::CalculateVisibility()
{
    if (_Stop)
    {
        return;
    }

    BytesContainer& Data = _DownloadResult.GetData(0, 0)->Data;

    int DetectedPixels = 0;

    for (int x = 0; x < Data.Length(); x += 4)
    {
        if (Data[x] != 0)
        {
            DetectedPixels++;
        }
    }

    _Callback(static_cast<float>(DetectedPixels) * 100.f / static_cast<float>(_Resolution2), _Target);
    _Finished = true;
}

void VisibilityWorker::OnRenderTask(RenderTask* RenderTask, GPUContext* GPUContext)
{
    if (_Stop)
    {
        return;
    }

    _Task->End.Unbind<VisibilityWorker, &VisibilityWorker::OnRenderTask>(this);
    _Task->Enabled = false;
    Camera->SetIsActive(false);
    VisibilityBox->SetIsActive(false);

    Task* DownloadTask = _Output->DownloadDataAsync(_DownloadResult);
    if (DownloadTask == nullptr)
    {
        ULOG_WARN_STR("Cannot create download async task.");
        return;
    }

    DownloadTask->ContinueWith(_DownloadFunc, this);
    DownloadTask->Start();
}

void VisibilityWorker::OnEnable()
{
    _Resolution2 = _Resolution * _Resolution;
    VisibilityBox->SetIsActive(false);
    Camera->SetIsActive(false);

    _DownloadFunc.Bind<VisibilityWorker, &VisibilityWorker::CalculateVisibility>(this);

    _Output = GPUTexture::New();

    GPUTextureDescription desc = GPUTextureDescription::New2D(
        _Resolution,
        _Resolution,
        PixelFormat::R8G8B8A8_UNorm);
    _Output->Init(desc);

    _Task = New<SceneRenderTask>();
    _Task->View.Flags = ViewFlags::None;
    _Task->View.Mode = ViewMode::NoPostFx;
    _Task->View.RenderLayersMask = Camera->RenderLayersMask;
    _Task->View.IsSingleFrame = true;
    _Task->Order = -100;
    _Task->Camera = Camera;
    _Task->Output = _Output;
}

void VisibilityWorker::OnDisable()
{
    if (_Task != nullptr)
    {
        _Task->Enabled = false;
        Delete(_Task);
    }

    if (_Output != nullptr)
    {
        _Output->ReleaseGPU();
        Delete(_Output);
    }
}

void VisibilityWorker::Queue(Vector3 Origin, Actor* Target, Function<void(float, Actor*)> Callback)
{
    _Finished = false;
    _Callback = Callback;
    _Target = Target;
    Camera->SetFieldOfView(1.0f);
    Camera->SetPosition(Origin);

    BoundingBox Box = Target->GetBoxWithChildren();

    Camera->LookAt(Box.GetCenter(), Vector3::Up);

    Vector3 Corners[8];
    Box.GetCorners(Corners);

    while (true)
    {
        bool AllVisible = true;
        for (int i = 0; i < 8; ++i)
        {
            if (!Camera->IsPointOnView(Corners[i]))
            {
                AllVisible = false;
                break;
            }
        }
        if (AllVisible)
        {
            break;
        }
        else
        {
            Camera->SetFieldOfView(Camera->GetFieldOfView() + 1.0f);
        }
    }

    VisibilityBox->SetIsActive(true);
    VisibilityBox->SetScale(Box.GetSize() * 0.01f);
    VisibilityBox->SetPosition(Box.GetCenter());
    Camera->SetIsActive(true);

    _Task->End.Bind<VisibilityWorker, &VisibilityWorker::OnRenderTask>(this);
    _Task->Enabled = true;
}
