#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actors/Camera.h>
#include <Engine/Engine/Time.h>
#include <Engine/Level/Actors/StaticModel.h>
#include <Engine/Content/Content.h>
#include <Engine/Level/Level.h>
#include <Engine/Graphics/Textures/GPUTexture.h>
#include <Engine/Graphics/RenderTask.h>
#include <Engine/Graphics/Textures/TextureData.h>
#include <Engine/Threading/Task.h>
#include <Engine/Threading/ThreadPoolTask.h>
#include <Engine/Core/Delegate.h>
#include <Engine/Profiler/Profiler.h>

#include <Game/Util/Defines.h>

API_CLASS() class GAME_API PlayerVisibilityCamera : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerVisibilityCamera);
private:
    
    Array<Color32> Pixels;
    Function<void()> DownloadFunc;
    int _ResolutionX = 48;
    int _ResolutionY = 48;
    TextureData DownloadResult;
    GPUTexture* _Output = nullptr;
    SceneRenderTask* _Task = nullptr;

public:

    int CurrentChild = 0;
    float Timer = 2.0f;
    float Counter = 0.0f;

    void CalculateVisibility();
    void OnRenderTask(RenderTask* RenderTask, GPUContext* GPUContext);

    API_FIELD() ScriptingObjectReference<Camera> Camera;
    API_FIELD() ScriptingObjectReference<StaticModel> VisibilityBox;
    API_FIELD() Array<Actor*> Children;

    void OnEnable() override;
    void OnDisable() override;
    void OnUpdate() override;
};
