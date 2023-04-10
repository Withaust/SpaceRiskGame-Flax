#include "VisibilityGPU.h"

Vector3 VisibilityTask::GetOrigin() const
{
    return _Origin;
}

Actor* VisibilityTask::GetTarget() const
{
    return _Target;
}

Function<void(float, Actor*)> VisibilityTask::GetCallback() const
{
    return _Callback;
}

VisibilityTask::VisibilityTask(Vector3 Origin, Actor* Target, Function<void(float, Actor*)> Callback)
    : _Origin(Origin),
    _Target(Target),
    _Callback(Callback)
{

}

VisibilityGPU::VisibilityGPU(const SpawnParams& params)
    : ISystem(params)
{
    _tickUpdate = true;
}

void VisibilityGPU::OnEnable()
{
    for (int i = 0; i < WorkerCount; ++i)
    {
        Actor* NewWorker = PrefabManager::SpawnPrefab(WorkerPrefab);
        NewWorker->SetParent(GetActor());
        Workers.Push(NewWorker->GetScript<VisibilityWorker>());
    }
}

void VisibilityGPU::OnDisable()
{
    for (int i = 0; i < Workers.Count(); ++i)
    {
        Workers[i]->Stop();
        Workers[i]->DeleteObject();
    }

    Workers.Clear();
}

void VisibilityGPU::OnUpdate()
{
    if (Tasks.Count() == 0)
    {
        return;
    }

    for (int i = 0; i < Workers.Count(); ++i)
    {
        if (!Workers[i]->IsFinished())
        {
            continue;
        }

        VisibilityTask& Task = Tasks.PeekFront();
        Workers[i]->Queue(Task.GetOrigin(), Task.GetTarget(), Task.GetCallback());
        Tasks.PopFront();
    }
}

void VisibilityGPU::Queue(Vector3 Origin, Actor* Target, Function<void(float, Actor*)> Callback)
{
    VisibilityTask NewTask(Origin, Target, Callback);
    Tasks.PushBack(NewTask);
}
