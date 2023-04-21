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

#include <Game/Shared/Utils/Defines.h>

API_CLASS() class GAME_API VisibilityWorker : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(VisibilityWorker);
private:

    Function<void()> _downloadFunc;
    int _resolution = 48;
    int _resolution2 = 0;
    TextureData _downloadResult;
    GPUTexture* _output = nullptr;
    SceneRenderTask* _task = nullptr;
    Actor* _target = nullptr;
    Function<void(float, Actor*)> _callback;
    bool _finished = true;
    bool _stop = false;

public:

    bool IsFinished();
    void Stop();

    void CalculateVisibility();
    void OnRenderTask(RenderTask* renderTask, GPUContext* gpuContext);

    API_FIELD() ScriptingObjectReference<Camera> Camera;
    API_FIELD() ScriptingObjectReference<StaticModel> VisibilityBox;

    void OnEnable() override;
    void OnDisable() override;
    void Queue(Vector3 origin, Actor* target, Function<void(float, Actor*)> callback);
};
