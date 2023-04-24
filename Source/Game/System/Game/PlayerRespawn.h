#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Prefabs/PrefabManager.h>
#include <Engine/Level/Prefabs/Prefab.h>
#include <Engine/Content/SoftAssetReference.h>
#include <Engine/Networking/NetworkManager.h>
#include <Engine/Core/Collections/HashSet.h>
#include <Engine/Core/RandomStream.h>

#include <Game/Shared/Utils/Defines.h>
#include <Game/System/Core/Networking.h>
#include <Game/Dynamic/Player/PlayerNetworking.h>
#include <Game/Level/PlayerSpawn.h>

API_CLASS() class GAME_API PlayerRespawn : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerRespawn);
    USINGLETON(PlayerRespawn);

    friend class PlayerSpawn;

private:

    Dictionary<NetworkClient*, Actor*> _players;
    Array<PlayerSpawn*> _spawns;

public:

    API_FIELD() SoftAssetReference<Prefab> PlayerPrefab;

    void Register(PlayerSpawn* spawn);
    void Unregister(PlayerSpawn* spawn);

    void OnPlayerConnected(NetworkClient* client) override;
    void OnPlayerDisconnected(NetworkClient* client) override;
};
