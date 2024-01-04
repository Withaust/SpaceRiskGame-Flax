#include "PlayerManager.h"

UIMPL_SINGLETON(PlayerManager)

PlayerManager::PlayerManager(const SpawnParams& params)
    : ISystem(params)
{
}

const Dictionary<uint32, ScriptingObjectReference<Entity>> PlayerManager::GetPlayers() const
{
    return _players;
}

ScriptingObjectReference<Entity> PlayerManager::GetPlayer(uint32 id)
{
    if (!_players.ContainsKey(id))
    {
        return {};
    }
    return _players[id];
}

ScriptingObjectReference<PawnInfo> PlayerManager::GetPlayerPawnInfo(uint32 id)
{
    if (!_players.ContainsKey(id))
    {
        return {};
    }
    return _players[id]->GetComponent<PawnInfo>();
}

void PlayerManager::SetOurPlayer(ScriptingObjectReference<Entity> actor)
{
    _ourPlayer = actor;
}

ScriptingObjectReference<Entity> PlayerManager::GetOurPlayer()
{
    return _ourPlayer;
}

void PlayerManager::Register(uint32 id, ScriptingObjectReference<Entity> actor)
{
    _players[id] = actor;
}

void PlayerManager::Unregister(uint32 id)
{
    _players.Remove(id);
}

void PlayerManager::OnPlayerConnected(NetworkClient* client)
{
    auto spawn = PlayerRespawns::Instance->GetRandomSpawn();
    Actor* newPlayer = Networking::Instance->SpawnPrefab(PlayerPrefab, GetActor(), client->ClientId, spawn->GetActor()->GetPosition(), spawn->GetActor()->GetOrientation());
}

void PlayerManager::OnPlayerDisconnected(NetworkClient* client)
{
    if (!_players.ContainsKey(client->ClientId))
    {
        return;
    }
    Networking::Instance->DespawnPrefab(_players[client->ClientId]);
}

void PlayerManager::OnDeinitialize()
{
    UDEINIT_SINGLETON();
}
