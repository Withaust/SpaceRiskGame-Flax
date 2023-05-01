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
    static Networking* Get() { return CoreInstance::Instance()->Get<Networking>(); }

private:

    String _windowTitle;
    bool _gameStarted = false;
    bool _isHosting = false;
    HashSet<ScriptingObject*> _immediateOwner;

public:

    // TODO: This is dumb fix, I hope this issue will be resolved
    // https://github.com/FlaxEngine/FlaxEngine/issues/1066
    bool CheckImmediateOwnership(ScriptingObject* target);

    void OnNetworkStateChanged();
    void OnNetworkClientConnected(NetworkClient* client);
    void OnNetworkClientDisconnected(NetworkClient* client);

    void OnInitialize() override;
    void OnDeinitialize() override;

    void StartGame();

    Actor* SpawnPrefab(Prefab* prefab, Actor* parent, uint32 ownerId = NetworkManager::LocalClientId, const Vector3& position = {}, const Quaternion& rotation = {});
    void DespawnPrefab(Actor* target);

    void StartReplicating(ScriptingObject* target);
    void StopReplicating(ScriptingObject* target);
    void DespawnReplicating(ScriptingObject* target);
};
