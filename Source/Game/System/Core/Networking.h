#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/NetworkManager.h>
#include <Engine/Networking/NetworkReplicator.h>
#include <Engine/Networking/NetworkStream.h>
#include <Engine/Networking/INetworkSerializable.h>
#include <Engine/Level/Level.h>
#include <Engine/Level/Scene/Scene.h>
#include <Engine/Level/Prefabs/Prefab.h>
#include <Engine/Level/Prefabs/PrefabManager.h>
#include <Engine/Engine/Engine.h>
#include <Engine/Networking/NetworkSettings.h>
#include <Engine/Engine/Engine.h>
#include <Engine/Platform/Window.h>
#include <Engine/Scripting/Enums.h>
#include <Engine/Networking/INetworkObject.h>
#include <Engine/Core/Collections/HashSet.h>
#include <Engine/Serialization/MemoryWriteStream.h>

#include <Game/Shared/SleepBlock.h>
#include <Game/System/Core/LaunchArgs.h>
#include <Game/System/Core/Logger.h>
#include <Game/Shared/Entity.h>
#include <Game/System/Core/SyncInfo.h>


API_CLASS() class GAME_API Networking : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Networking);
    friend class Entity;
    friend class CustomHierarchy;
    friend class SyncInfo;
    friend class INetworkedObject;
    friend class Core;
    friend class EngineHelper;

private:
    bool _gameStarted = false;
    bool _isHosting = false;

    NetworkStream* _stream = nullptr;

    struct SyncEvent
    {
        NetworkClient* client;
        ScriptingObjectReference<ScriptingObject> object;
    };

    HashSet<ScriptingObjectReference<ScriptingObject>> _spawnList;
    Array<SyncEvent> _syncEvents;

    API_FUNCTION() void AddReplicatedSystem(ScriptingObjectReference<ScriptingObject> obj);

public:

    API_FIELD() static Networking* Instance;

    void OnNetworkStateChanged();
    void OnNetworkClientConnected(NetworkClient* client);
    void OnNetworkClientDisconnected(NetworkClient* client);
    void OnSynced(NetworkClient* client);

    void BindEvents();
    void UnbindEvents();

    void OnInitialize() override;
    void OnDeinitialize() override;
    void OnUpdate() override;

    API_FUNCTION() void StartGame();

    API_FUNCTION() ScriptingObjectReference<Entity> SpawnPrefab(Prefab* prefab, Actor* parent, uint32 ownerId = NetworkManager::ServerClientId, const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion());
    API_FUNCTION() void DespawnPrefab(ScriptingObjectReference<Entity> target);

    API_FUNCTION() void StartReplicating(ScriptingObjectReference<Entity> target);
    API_FUNCTION() void StopReplicating(ScriptingObjectReference<Entity> target);
    API_FUNCTION() void DespawnReplicating(ScriptingObjectReference<Entity> target);

    API_FUNCTION() Guid SerializeEntity(ScriptingObjectReference<Entity> target);
    API_FUNCTION() ScriptingObjectReference<Entity> DeserializeEntity(Guid id, NetworkRpcParams rpcParams);
    API_FUNCTION() Span<uint32> GetClientIdsExcept(uint32 exception);
};
