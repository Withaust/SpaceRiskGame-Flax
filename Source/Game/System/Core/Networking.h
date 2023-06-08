#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/NetworkManager.h>
#include <Engine/Networking/NetworkReplicator.h>
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

API_CLASS() class GAME_API Networking : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Networking);

private:
    bool _gameStarted = false;
    bool _isHosting = false;

public:

    void OnNetworkStateChanged();
    void OnNetworkClientConnected(NetworkClient* client);
    void OnNetworkClientDisconnected(NetworkClient* client);

    void OnInitialize() override;
    void OnDeinitialize() override;

    API_FUNCTION() void StartGame();

    API_FUNCTION() Actor* SpawnPrefab(Prefab* prefab, Actor* parent, uint32 ownerId = NetworkManager::ServerClientId, const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion());
    API_FUNCTION() void DespawnPrefab(Actor* target);

    API_FUNCTION() void StartReplicating(ScriptingObject* target);
    API_FUNCTION() void StopReplicating(ScriptingObject* target);
    API_FUNCTION() void DespawnReplicating(ScriptingObject* target);
};
