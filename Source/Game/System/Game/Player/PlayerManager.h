#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/Shared/Defines.h>
#include <Game/System/Game/Player/PlayerRespawns.h>
#include <Game/Dynamic/Player/PlayerInfo.h>

API_CLASS() class GAME_API PlayerManager : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerManager);
    friend class PlayerInfo;

private:
    
    ScriptingObjectReference<Entity> _ourPlayer;
    Dictionary<uint32, ScriptingObjectReference<Entity>> _players;

public:
    API_FIELD() static PlayerManager* Instance;

    API_FIELD() SoftAssetReference<Prefab> PlayerPrefab;

    API_FUNCTION() ScriptingObjectReference<PlayerInfo> GetPlayerInfo(uint32 id);

    API_FUNCTION() void SetOurPlayer(ScriptingObjectReference<Entity> actor);
    API_FUNCTION() ScriptingObjectReference<Entity> GetOurPlayer();

    API_FUNCTION() void Register(uint32 id, ScriptingObjectReference<Entity> actor);
    API_FUNCTION() void Unregister(uint32 id);

    void OnPlayerConnected(NetworkClient* client) override;
    void OnPlayerDisconnected(NetworkClient* client) override;
};
