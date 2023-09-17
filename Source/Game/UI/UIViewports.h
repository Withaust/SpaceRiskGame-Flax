#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Graphics/Textures/GPUTexture.h>
#include <Engine/Graphics/GPUContext.h>
#include <Engine/Graphics/Textures/TextureData.h>
#include <Engine/Graphics/Textures/TextureUtils.h>
#include <Engine/Threading/Task.h>
#include <Engine/Graphics/RenderTask.h>
#include <Engine/Level/Actors/Camera.h>
#include <Engine/Content/Content.h>
#include <Engine/Core/Collections/HashSet.h>
#include <Engine/Core/RandomStream.h>

API_CLASS() class GAME_API UIViewports : public Script
{
    DECLARE_SCRIPTING_TYPE(UIViewports);
private:

    struct UIViewport
    {
        GPUTexture* GPUTexture = nullptr;
        SceneRenderTask* Task = nullptr;
    };

    Dictionary<StringAnsi, UIViewport> _viewports;
    Dictionary<uintptr_t, StringAnsi> _handles;

public:

    void OnPreRender(GPUContext* context, RenderContext& renderContext)
    {
        renderContext.Task->Camera->SetIsActive(true);
    }

    void OnPostRender(GPUContext* context, RenderContext& renderContext)
    {
        renderContext.Task->Camera->SetIsActive(false);
    }

    void CreateViewport(StringAnsi name, ViewFlags flags, ViewMode mode, int32 order)
    {
        UIViewport viewport;
        viewport.Task = New<SceneRenderTask>();
        viewport.Task->View.Flags = flags;
        viewport.Task->View.Mode = mode;
        viewport.Task->Order = order;
        viewport.Task->PreRender.Bind<UIViewports, &UIViewports::OnPreRender>(this);
        viewport.Task->PostRender.Bind<UIViewports, &UIViewports::OnPostRender>(this);
        _viewports[name] = viewport;
    }

    void UpdateViewportCamera(StringAnsi name, Camera* camera)
    {
        if (_viewports.ContainsKey(name))
        {
            auto& targetViewport = _viewports.At(name);
            targetViewport.Task->View.RenderLayersMask = camera->RenderLayersMask;
            targetViewport.Task->Camera = camera;
            targetViewport.Task->Enabled = true;
        }
    }

    void UpdateViewportSize(StringAnsi name, Float2 size)
    {
        if (_viewports.ContainsKey(name))
        {
            auto& targetViewport = _viewports.At(name);

            if (targetViewport.GPUTexture)
            {
                targetViewport.GPUTexture->ReleaseGPU();
                Delete(targetViewport.GPUTexture);
            }

            targetViewport.GPUTexture = GPUTexture::New();

            GPUTextureDescription desc = GPUTextureDescription::New2D(
                static_cast<int32>(size.X),
                static_cast<int32>(size.Y),
                PixelFormat::R8G8B8A8_UNorm);
            targetViewport.GPUTexture->Init(desc);
            targetViewport.Task->Output = targetViewport.GPUTexture;
        }
    }

    GPUTexture* OnLoadTexture(uintptr_t id, Float2& dimentions, const String& path)
    {
        StringAnsi file = StringUtils::GetFileNameWithoutExtension(path).ToStringAnsi();
        if (file.StartsWith("V_"))
        {
            if (_viewports.ContainsKey(file))
            {
                auto& targetViewport = _viewports.At(file);
                dimentions = targetViewport.GPUTexture->Size();
                _handles[id] = file;
                return targetViewport.GPUTexture;
            }
            else
            {
                return nullptr;
            }
        }
        return nullptr;
    }

    bool OnReleaseTexture(uintptr_t id)
    {
        if (_viewports.Count() == 0)
        {
            return true;
        }

        StringAnsi name;

        if (!_handles.TryGet(id, name))
        {
            return false;
        }

        if (_viewports.ContainsKey(name))
        {
            UIViewport& viewport = _viewports.At(name);
            ClearViewport(viewport);
            _viewports.Remove(name);
            return true;
        }

        return false;
    }

    void ClearViewport(UIViewport& viewport)
    {
        if (viewport.Task)
        {
            viewport.Task->Enabled = false;
            Delete(viewport.Task);
        }

        if (viewport.GPUTexture)
        {
            viewport.GPUTexture->ReleaseGPU();
            Delete(viewport.GPUTexture);
        }
    }

    void ClearViewports()
    {
        for (auto& viewport : _viewports)
        {
            ClearViewport(viewport.Value);
        }
    }
};
