#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actors/EmptyActor.h>
#include <Engine/Core/Collections/Array.h>
#include <Engine/Core/Collections/RingBuffer.h>
#include <Engine/Level/Prefabs/PrefabManager.h>
#include <Engine/Level/Prefabs/Prefab.h>
#include <Engine/Content/SoftAssetReference.h>

#include <Game/Shared/Defines.h>
#include <Game/Level/VisibilityWorker.h>

class VisibilityTask
{
    Vector3 _origin;
    Actor* _target;
    Function<void(float, Actor*)> _callback;

public:

    Vector3 GetOrigin() const;
    Actor* GetTarget() const;
    Function<void(float, Actor*)> GetCallback() const;

    VisibilityTask() = delete;
    VisibilityTask(Vector3 origin, Actor* target, Function<void(float, Actor*)> callback);
};

API_CLASS() class GAME_API VisibilityGPU : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(VisibilityGPU);

private:
    Array<ScriptingObjectReference<VisibilityWorker>> _workers;
    RingBuffer<VisibilityTask> _tasks;

public:
    API_FIELD() static VisibilityGPU* Instance;

    API_FIELD(Attributes = "Cmd") int WorkerCount = 4;
    API_FIELD() SoftAssetReference<Prefab> WorkerPrefab;

    void OnEnable() override;
    void OnDisable() override;
    void OnUpdate() override;
    API_FUNCTION() void Queue(Vector3 origin, Actor* target, Function<void(float, Actor*)> callback);
};
