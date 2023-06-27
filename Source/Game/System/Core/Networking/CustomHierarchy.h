#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/NetworkReplicationHierarchy.h>
#include <Engine/Networking/NetworkReplicator.h>
#include <Engine/Networking/NetworkManager.h>
#include <Engine/Core/Collections/ArrayExtensions.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/IComponent.h>
#include <Game/System/Core/Networking/Networking.h>

API_CLASS() class GAME_API CustomHierarchy : public NetworkReplicationHierarchy
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(CustomHierarchy, NetworkReplicationHierarchy);
    friend class Networking;
    friend class IComponent;

private:
    struct SyncEvent
    {
        NetworkClient* client;
        ScriptingObjectReference<IComponent> object;
    };

    NetworkReplicationGridNode _grid;
    HashSet<ScriptingObjectReference<IComponent>> _spawnList;
    Array<SyncEvent> _syncEvents;

    void OnClientConnected(NetworkClient* client);

public:
    void AddObject(NetworkReplicationHierarchyObject obj) override;
    bool RemoveObject(ScriptingObject* obj) override;
    bool GetObject(ScriptingObject* obj, NetworkReplicationHierarchyObject& result) override;
    void Update(NetworkReplicationHierarchyUpdateResult* result) override;
};
