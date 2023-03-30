#include "PlayerVisibilityCamera.h"

PlayerVisibilityCamera::PlayerVisibilityCamera(const SpawnParams& params)
    : Script(params)
{
    _tickUpdate = true;
}

void PlayerVisibilityCamera::CalculateVisibility()
{
    PROFILE_CPU();
    const TextureMipData* Data = DownloadResult.GetData(0, 0);    
    Data->GetPixels(Pixels, _ResolutionX, _ResolutionY, PixelFormat::R8G8B8A8_UNorm);

    int ResolutionAll = _ResolutionX * _ResolutionY;
    int DetectedPixels = 0;

    for (int x = 0; x < ResolutionAll; ++x)
    {
        if (Pixels[x].R != 0)
        {
            DetectedPixels++;
        }
    }

    float Percentage = static_cast<float>(DetectedPixels) * 100.f / static_cast<float>(ResolutionAll);

    ULOG_DEBUG("CPU Detected pixels: {0}", DetectedPixels);
    ULOG_DEBUG("CPU Visibility percentage: {0} %", Percentage);
}

void PlayerVisibilityCamera::OnRenderTask(RenderTask* RenderTask, GPUContext* GPUContext)
{
    PROFILE_CPU();
    _Task->End.Unbind<PlayerVisibilityCamera, &PlayerVisibilityCamera::OnRenderTask>(this);
    _Task->Enabled = false;
    Camera->SetIsActive(false);
    VisibilityBox->SetIsActive(false);

    Task* DownloadTask = _Output->DownloadDataAsync(DownloadResult);
    if (DownloadTask == nullptr)
    {
        ULOG_WARN_STR("Cannot create download async task.");
        Delete<Task>(DownloadTask);
        return;
    }

    DownloadTask->ContinueWith(DownloadFunc, this);
    DownloadTask->Start();
}

void PlayerVisibilityCamera::OnEnable()
{
    VisibilityBox->SetIsActive(false);
    Camera->SetIsActive(false);
    Pixels.Resize(_ResolutionX * _ResolutionY);

    DownloadFunc.Bind<PlayerVisibilityCamera, &PlayerVisibilityCamera::CalculateVisibility>(this);

    _Output = GPUTexture::New();

    GPUTextureDescription desc = GPUTextureDescription::New2D(
        _ResolutionX,
        _ResolutionY,
        PixelFormat::R8G8B8A8_UNorm);
    _Output->Init(desc);

    _Task = New<SceneRenderTask>();
    _Task->View.Flags = ViewFlags::None;
    _Task->View.RenderLayersMask = Camera->RenderLayersMask;
    _Task->View.IsSingleFrame = true;
    _Task->Order = -100;
    _Task->Camera = Camera;
    _Task->Output = _Output;
}

void PlayerVisibilityCamera::OnDisable()
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

void PlayerVisibilityCamera::OnUpdate()
{
    Counter += Time::GetDeltaTime();
    if (Counter <= Timer)
    {
        return;
    }
    Counter = 0.0f;
    CurrentChild++;

    PROFILE_CPU();

    if (CurrentChild >= Children.Count())
    {
        CurrentChild = 0;
    }

    Camera->SetFieldOfView(1.0f);

    Actor* Child = Children[CurrentChild];
    BoundingBox Box = Child->GetBoxWithChildren();

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

    _Task->End.Bind<PlayerVisibilityCamera, &PlayerVisibilityCamera::OnRenderTask>(this);
    _Task->Enabled = true;
}
