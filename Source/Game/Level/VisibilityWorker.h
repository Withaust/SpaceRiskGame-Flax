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

#include <Game/Utils/Defines.h>

API_CLASS() class GAME_API VisibilityWorker : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(VisibilityWorker);
private:

    Function<void()> _DownloadFunc;
    int _Resolution = 48;
    int _Resolution2 = 0;
    TextureData _DownloadResult;
    GPUTexture* _Output = nullptr;
    SceneRenderTask* _Task = nullptr;
    Actor* _Target = nullptr;
    Function<void(float, Actor*)> _Callback;
    bool _Finished = true;
    bool _Stop = false;

public:

    bool IsFinished();
    void Stop();

    void CalculateVisibility();
    void OnRenderTask(RenderTask* RenderTask, GPUContext* GPUContext);

    API_FIELD() ScriptingObjectReference<Camera> Camera;
    API_FIELD() ScriptingObjectReference<StaticModel> VisibilityBox;

    void OnEnable() override;
    void OnDisable() override;
    void Queue(Vector3 Origin, Actor* Target, Function<void(float, Actor*)> Callback);
};
