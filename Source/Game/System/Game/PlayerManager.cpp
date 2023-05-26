#include "PlayerManager.h"

#include <Engine/Serialization/JsonWriters.h>

ScriptingObjectReference<Actor> PlayerManager::_ourPlayer;
Dictionary<uint32, ScriptingObjectReference<Actor>> PlayerManager::_players;

PlayerManager::PlayerManager(const SpawnParams& params)
    : ISystem(params)
{
}

ScriptingObjectReference<PlayerInfo> PlayerManager::GetPlayerInfo(uint32 id)
{
    if (!_players.ContainsKey(id))
    {
        return nullptr;
    }
    return _players[id]->GetScript<PlayerInfo>();
}

void PlayerManager::SetOurPlayer(ScriptingObjectReference<Actor> actor)
{
    _ourPlayer = actor;
}

ScriptingObjectReference<Actor> PlayerManager::GetOurPlayer()
{
    return _ourPlayer;
}

void PlayerManager::Register(uint32 id, ScriptingObjectReference<Actor> actor)
{
    _players[id] = actor;
}

void PlayerManager::Unregister(uint32 id)
{
    _players.Remove(id);
}

void PlayerManager::OnPlayerConnected(NetworkClient* client)
{
    auto spawn = Core::Get<PlayerRespawns>()->GetRandomSpawn();
    Actor* newPlayer = Core::Get<Networking>()->SpawnPrefab(PlayerPrefab, GetActor(), client->ClientId, spawn->GetActor()->GetPosition(), spawn->GetActor()->GetOrientation());
}

void PlayerManager::OnPlayerDisconnected(NetworkClient* client)
{
    if (!_players.ContainsKey(client->ClientId))
    {
        return;
    }
    Core::Get<Networking>()->DespawnPrefab(_players[client->ClientId]);
}
