#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/Shared/Utils/Defines.h>
#include <Game/System/Game/PlayerRespawns.h>
#include <Game/Dynamic/Player/PlayerInfo.h>

API_CLASS() class GAME_API PlayerManager : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerManager);
    friend class PlayerInfo;

private:
    
    ScriptingObjectReference<Actor> _ourPlayer;
    Dictionary<uint32, ScriptingObjectReference<Actor>> _players;

public:

    API_FIELD() SoftAssetReference<Prefab> PlayerPrefab;

    API_FUNCTION() ScriptingObjectReference<PlayerInfo> GetPlayerInfo(uint32 id);

    void SetOurPlayer(ScriptingObjectReference<Actor> actor);
    API_FUNCTION() ScriptingObjectReference<Actor> GetOurPlayer();

    void Register(uint32 id, ScriptingObjectReference<Actor> actor);
    void Unregister(uint32 id);

    void OnPlayerConnected(NetworkClient* client) override;
    void OnPlayerDisconnected(NetworkClient* client) override;
};
