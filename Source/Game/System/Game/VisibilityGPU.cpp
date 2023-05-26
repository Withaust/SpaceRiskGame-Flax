#include "VisibilityGPU.h"

Array<VisibilityWorker*> VisibilityGPU::_workers;
RingBuffer<VisibilityTask> VisibilityGPU::_tasks;

Vector3 VisibilityTask::GetOrigin() const
{
    return _origin;
}

Actor* VisibilityTask::GetTarget() const
{
    return _target;
}

Function<void(float, Actor*)> VisibilityTask::GetCallback() const
{
    return _callback;
}

VisibilityTask::VisibilityTask(Vector3 origin, Actor* target, Function<void(float, Actor*)> callback)
    : _origin(origin),
    _target(target),
    _callback(callback)
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
        Actor* newWorker = PrefabManager::SpawnPrefab(WorkerPrefab, GetActor());
        _workers.Push(newWorker->GetScript<VisibilityWorker>());
    }
}

void VisibilityGPU::OnDisable()
{
    for (int i = 0; i < _workers.Count(); ++i)
    {
        _workers[i]->Stop();
        _workers[i]->DeleteObject();
    }

    _workers.Clear();
}

void VisibilityGPU::OnUpdate()
{
    if (_tasks.Count() == 0)
    {
        return;
    }

    for (int i = 0; i < _workers.Count(); ++i)
    {
        if (!_workers[i]->IsFinished())
        {
            continue;
        }

        VisibilityTask& task = _tasks.PeekFront();
        _workers[i]->Queue(task.GetOrigin(), task.GetTarget(), task.GetCallback());
        _tasks.PopFront();
    }
}

void VisibilityGPU::Queue(Vector3 origin, Actor* target, Function<void(float, Actor*)> callback)
{
    VisibilityTask newTask(origin, target, callback);
    _tasks.PushBack(newTask);
}
