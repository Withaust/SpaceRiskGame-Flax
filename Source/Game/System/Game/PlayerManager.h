#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/Shared/Defines.h>
#include <Game/System/Game/PlayerRespawns.h>
#include <Game/Dynamic/Player/PlayerInfo.h>

API_CLASS() class GAME_API PlayerManager : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerManager);
    friend class PlayerInfo;

private:
    
    static ScriptingObjectReference<Actor> _ourPlayer;
    static Dictionary<uint32, ScriptingObjectReference<Actor>> _players;

public:

    API_FIELD() SoftAssetReference<Prefab> PlayerPrefab;

    API_FUNCTION() static ScriptingObjectReference<PlayerInfo> GetPlayerInfo(uint32 id);

    API_FUNCTION() static void SetOurPlayer(ScriptingObjectReference<Actor> actor);
    API_FUNCTION() static ScriptingObjectReference<Actor> GetOurPlayer();

    API_FUNCTION() static void Register(uint32 id, ScriptingObjectReference<Actor> actor);
    API_FUNCTION() static void Unregister(uint32 id);

    void OnPlayerConnected(NetworkClient* client) override;
    void OnPlayerDisconnected(NetworkClient* client) override;
};
