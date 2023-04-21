#include "PlayerRespawn.h"

PlayerRespawn::PlayerRespawn(const SpawnParams& params)
    : ISystem(params)
{
}

void PlayerRespawn::OnInitialize()
{
}

void PlayerRespawn::OnDeinitialize()
{
}

void PlayerRespawn::OnPlayerConnected(NetworkClient* client)
{
    Actor* newPlayer = PrefabManager::SpawnPrefab(PlayerPrefab, GetActor());
    _players[client] = newPlayer;

    if (NetworkManager::LocalClientId != client->ClientId)
    {
        NetworkReplicator::SpawnObject(newPlayer);
        for (int i = 0; i < newPlayer->Scripts.Count(); ++i)
        {
            NetworkReplicator::SpawnObject(newPlayer->Scripts[i]);
        }
        NetworkReplicator::SetObjectOwnership(newPlayer, client->ClientId, NetworkObjectRole::ReplicatedSimulated, true);
    }
    else
    {
        NetworkReplicator::SpawnObject(newPlayer);
        for (int i = 0; i < newPlayer->Scripts.Count(); ++i)
        {
            NetworkReplicator::SpawnObject(newPlayer->Scripts[i]);
        }
        NetworkReplicator::SetObjectOwnership(newPlayer, client->ClientId, NetworkObjectRole::OwnedAuthoritative, true);
        newPlayer->GetScript<PlayerNetworking>()->ClaimAuthority();
    }

    Level::SpawnActor(newPlayer, GetActor());

    // TODO: Teleport to the nearest spawnpoint
    newPlayer->SetPosition(Vector3());
}

void PlayerRespawn::OnPlayerDisconnected(NetworkClient* client)
{
    if (!_players.ContainsKey(client))
    {
        return;
    }
    NetworkReplicator::DespawnObject(_players[client]);
    _players.Remove(client);
}
