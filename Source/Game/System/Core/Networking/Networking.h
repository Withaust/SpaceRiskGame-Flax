#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/NetworkManager.h>
#include <Engine/Networking/NetworkReplicator.h>
#include <Engine/Networking/NetworkStream.h>
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

#include <Game/System/Core/LaunchArgs.h>
#include <Game/System/Core/Logger.h>
#include <Game/Shared/Entity.h>
#include <Game/System/Core/Networking/CustomHierarchy.h>

API_CLASS() class GAME_API Networking : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Networking);
    friend class Entity;
    friend class CustomHierarchy;
    friend class ISpawnSync;

private:
    bool _gameStarted = false;
    bool _isHosting = false;
    CustomHierarchy* _hierarchy = nullptr;
    NetworkStream* _stream = nullptr;
    int _syncFrame = 0;
public:
    API_FIELD() static Networking* Instance;

    void OnNetworkStateChanged();
    void OnNetworkClientConnected(NetworkClient* client);
    void OnNetworkClientDisconnected(NetworkClient* client);

    API_FUNCTION(NetworkRpc = "Server, Reliable") void AskForSync(NetworkRpcParams info = NetworkRpcParams());
    void RequestSpawnSync();

    void BindEvents();
    void UnbindEvents();

    void OnInitialize() override;
    void OnDeinitialize() override;

    API_FUNCTION() void StartGame();

    API_FUNCTION() ScriptingObjectReference<Entity> SpawnPrefab(Prefab* prefab, Actor* parent, uint32 ownerId = NetworkManager::ServerClientId, const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion());
    API_FUNCTION() void DespawnPrefab(ScriptingObjectReference<Entity> target);

    API_FUNCTION() void StartReplicating(Entity* target);
    API_FUNCTION() void StopReplicating(Entity* target);
    API_FUNCTION() void DespawnReplicating(Entity* target);
};
