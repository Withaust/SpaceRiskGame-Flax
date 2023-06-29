#include "VisibilityGPUWorker.h"

VisibilityGPUWorker::VisibilityGPUWorker(const SpawnParams& params)
    : Script(params)
{
    _tickUpdate = true;
}

bool VisibilityGPUWorker::IsFinished()
{
    return _finished;
}

void VisibilityGPUWorker::Stop()
{
    _stop = true;
}

void VisibilityGPUWorker::CalculateVisibility()
{
    if (_stop)
    {
        return;
    }

    BytesContainer& data = _downloadResult.GetData(0, 0)->Data;

    int detectedPixels = 0;

    for (int x = 0; x < data.Length(); x += 4)
    {
        if (data[x] != 0)
        {
            detectedPixels++;
        }
    }

    _callback(static_cast<float>(detectedPixels) * 100.f / static_cast<float>(_resolution2), _target);
    _finished = true;
}

void VisibilityGPUWorker::OnRenderTask(RenderTask* renderTask, GPUContext* gpuContext)
{
    if (_stop)
    {
        return;
    }

    _task->End.Unbind<VisibilityGPUWorker, &VisibilityGPUWorker::OnRenderTask>(this);
    _task->Enabled = false;
    Camera->SetIsActive(false);
    VisibilityBox->SetIsActive(false);

    Task* downloadTask = _output->DownloadDataAsync(_downloadResult);
    if (!downloadTask)
    {
        UERR_STR("Cannot create download async task.");
        return;
    }

    downloadTask->ContinueWith(_downloadFunc, this);
    downloadTask->Start();
}

void VisibilityGPUWorker::OnEnable()
{
    _resolution2 = _resolution * _resolution;
    VisibilityBox->SetIsActive(false);
    Camera->SetIsActive(false);

    _downloadFunc.Bind<VisibilityGPUWorker, &VisibilityGPUWorker::CalculateVisibility>(this);

    _output = GPUTexture::New();

    GPUTextureDescription desc = GPUTextureDescription::New2D(
        _resolution,
        _resolution,
        PixelFormat::R8G8B8A8_UNorm);
    _output->Init(desc);

    _task = New<SceneRenderTask>();
    _task->View.Flags = ViewFlags::None;
    _task->View.Mode = ViewMode::NoPostFx;
    _task->View.RenderLayersMask = Camera->RenderLayersMask;
    _task->View.IsSingleFrame = true;
    _task->Order = 9999;
    _task->Camera = Camera;
    _task->Output = _output;
}

void VisibilityGPUWorker::OnDisable()
{
    if (_task)
    {
        _task->Enabled = false;
        Delete(_task);
    }

    if (_output)
    {
        _output->ReleaseGPU();
        Delete(_output);
    }
}

void VisibilityGPUWorker::Queue(Vector3 origin, Actor* target, Function<void(float, Actor*)> callback)
{
    _finished = false;
    _callback = callback;
    _target = target;
    Camera->SetFieldOfView(1.0f);
    Camera->SetPosition(origin);

    BoundingBox Box = target->GetBoxWithChildren();

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

    _task->End.Bind<VisibilityGPUWorker, &VisibilityGPUWorker::OnRenderTask>(this);
    _task->Enabled = true;
}
