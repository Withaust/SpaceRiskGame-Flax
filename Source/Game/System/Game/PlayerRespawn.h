#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Prefabs/PrefabManager.h>
#include <Engine/Level/Prefabs/Prefab.h>
#include <Engine/Content/SoftAssetReference.h>
#include <Engine/Networking/NetworkManager.h>

#include <Game/Shared/Utils/Defines.h>
#include <Game/Dynamic/Player/PlayerNetworking.h>

API_CLASS() class GAME_API PlayerRespawn : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerRespawn);
    USINGLETON(PlayerRespawn);
private:

    Dictionary<NetworkClient*, Actor*> _Players;

public:

    API_FIELD() SoftAssetReference<Prefab> PlayerPrefab;
    API_FIELD() SoftAssetReference<Prefab> UIPrefab;

    void OnInitialize() override;
    void OnDeinitialize() override;
    void OnPlayerConnected(NetworkClient* client) override;
    void OnPlayerDisconnected(NetworkClient* client) override;
};
