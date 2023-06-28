#include "CustomHierarchy.h"
#include <Game/System/Game/Player/PlayerManager.h>

void CustomHierarchy::OnClientConnected(NetworkClient* client)
{
    for (const auto& spawn : _spawnList)
    {
        SyncEvent newEvent;
        newEvent.client = client;
        newEvent.object = spawn.Item;
        _syncEvents.Enqueue(newEvent);
    }
}

void CustomHierarchy::AddObject(NetworkReplicationHierarchyObject obj)
{
    ScriptingObjectReference<IComponent> component = Cast<IComponent>(obj.Object);
    if (component)
    {
        if (component->SyncOnlySpawn)
        {
            obj.ReplicationFPS = -1.0f;
            _spawnList.Add(component);
        }
    }

    Actor* actor = obj.GetActor();
    bool addedToGrid = false;
    if (actor && actor->HasStaticFlag(StaticFlags::Transform))
    {
        // Insert static objects into a grid for faster replication
        _grid.AddObject(obj);
        addedToGrid = true;
    }
    else
    {
        NetworkReplicationHierarchy::AddObject(obj);
    }
}

// Called by NetworkReplicator to remove object from hierarchy
bool CustomHierarchy::RemoveObject(ScriptingObject* obj)
{
    ScriptingObjectReference<IComponent> component = Cast<IComponent>(obj);
    if (component && component->SyncOnlySpawn)
    {
        _spawnList.Remove(component);
    }

    if (_grid.RemoveObject(obj))
    {
        return true;
    }

    return NetworkReplicationHierarchy::RemoveObject(obj);
}

bool CustomHierarchy::GetObject(ScriptingObject* obj, NetworkReplicationHierarchyObject& result)
{
    if (_grid.GetObject(obj, result))
    {
        return true;
    }
    if (GetObject(obj, result))
    {
        return true;
    }
    return false;
}

// Called every network update to gather objects for replication
void CustomHierarchy::Update(NetworkReplicationHierarchyUpdateResult* result)
{
    // Setup players locations for distance culling
    const auto& clients = NetworkManager::Clients;

    for (int32 i = 0; i < clients.Count(); i++)
    {
        auto entity = PlayerManager::Instance->GetPlayer(clients[i]->ClientId);
        if (entity)
        {
            result->SetClientLocation(i, entity->GetComponent<PlayerMovement>()->Controller->GetPosition());
        }
        else
        {
            result->SetClientLocation(i, Vector3::Zero);
        }
    }

    // Update hierarchy
    _grid.Update(result);
    NetworkReplicationHierarchy::Update(result);

    if (NetworkManager::IsClient())
    {
        Networking::Instance->RequestSpawnSync();
        return;
    }
    else if (_syncEvents.Count() == 0)
    {
        return;
    }

    SyncEvent sync = _syncEvents.Dequeue();

    if (!NetworkManager::Clients.Contains(sync.client))
    {
        return;
    }

    NetworkStream* stream = Networking::Instance->_stream;
    stream->Initialize();

    NetworkReplicator::InvokeSerializer(sync.object->GetTypeHandle(), sync.object, stream, true);

    Array<byte> data(stream->GetBuffer(), stream->GetLength());

    NetworkRpcParams params;
    uint32 ids[1] = { sync.client->ClientId };
    params.TargetIds = ToSpan(ids, ARRAY_COUNT(ids));
    sync.object->SendData(params, data);
}
