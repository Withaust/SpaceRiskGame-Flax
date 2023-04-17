#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actors/EmptyActor.h>
#include <Engine/Core/Collections/Array.h>
#include <Engine/Core/Collections/RingBuffer.h>
#include <Engine/Level/Prefabs/PrefabManager.h>
#include <Engine/Level/Prefabs/Prefab.h>
#include <Engine/Content/SoftAssetReference.h>

#include <Game/Shared/Utils/Defines.h>
#include <Game/Level/VisibilityWorker.h>

class VisibilityTask
{
    Vector3 _Origin;
    Actor* _Target;
    Function<void(float, Actor*)> _Callback;

public:

    Vector3 GetOrigin() const;
    Actor* GetTarget() const;
    Function<void(float, Actor*)> GetCallback() const;

    VisibilityTask() = delete;
    VisibilityTask(Vector3 Origin, Actor* Target, Function<void(float, Actor*)> Callback);
};

API_CLASS() class GAME_API VisibilityGPU : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(VisibilityGPU);
    USINGLETON(ISystem);

private:
    Array<VisibilityWorker*> Workers;
    RingBuffer<VisibilityTask> Tasks;

public:

    API_FIELD() int WorkerCount = 4;
    API_FIELD() SoftAssetReference<Prefab> WorkerPrefab;

    void OnEnable() override;
    void OnDisable() override;
    void OnUpdate() override;
    void Queue(Vector3 Origin, Actor* Target, Function<void(float, Actor*)> Callback);
};
