#include "PlayerRespawn.h"

PlayerRespawn::PlayerRespawn(const SpawnParams& params)
    : ISystem(params)
{
}

void PlayerRespawn::Register(PlayerSpawn* spawn)
{
    _spawns.Add(spawn);
}

void PlayerRespawn::Unregister(PlayerSpawn* spawn)
{
    _spawns.Remove(spawn);
}

void PlayerRespawn::OnPlayerConnected(NetworkClient* client)
{
    RandomStream random;
    random.GenerateNewSeed();
    int index = random.RandRange(0, _spawns.Count() - 1);
    PlayerSpawn* spawn = _spawns[index];

    UPRINT("Using spawn {0}", index);

    Actor* newPlayer = Networking::Get()->SpawnPrefab(PlayerPrefab, GetActor(), client->ClientId, spawn->GetActor()->GetPosition(), spawn->GetActor()->GetOrientation());
    _players[client] = newPlayer;
}

void PlayerRespawn::OnPlayerDisconnected(NetworkClient* client)
{
    if (!_players.ContainsKey(client))
    {
        return;
    }
    Networking::Get()->DespawnPrefab(_players[client]);
    _players.Remove(client);
}
