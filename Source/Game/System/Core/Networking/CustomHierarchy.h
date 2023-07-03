#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/NetworkReplicationHierarchy.h>
#include <Engine/Networking/NetworkReplicator.h>
#include <Engine/Networking/NetworkManager.h>
#include <Engine/Core/Collections/ArrayExtensions.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/IComponent.h>

API_CLASS() class GAME_API CustomHierarchy : public NetworkReplicationHierarchy
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(CustomHierarchy, NetworkReplicationHierarchy);
    friend class Networking;
    friend class Component;

private:
    struct SyncEvent
    {
        NetworkClient* client;
        ScriptingObjectReference<INetworkedObject> object;
    };

    NetworkReplicationGridNode _grid;
    HashSet<ScriptingObjectReference<INetworkedObject>> _spawnList;
    Array<SyncEvent> _syncEvents;

    void OnClientConnected(NetworkClient* client);

public:
    void AddObject(NetworkReplicationHierarchyObject obj) override;
    bool RemoveObject(ScriptingObject* obj) override;
    bool GetObject(ScriptingObject* obj, NetworkReplicationHierarchyObject& result) override;
    void Update(NetworkReplicationHierarchyUpdateResult* result) override;
};
