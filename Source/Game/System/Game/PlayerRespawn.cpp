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
    Actor* NewPlayer = PrefabManager::SpawnPrefab(PlayerPrefab, CoreInstance::Instance());
    _Players[client] = NewPlayer;

    if (NetworkManager::LocalClientId != client->ClientId)
    {
        NetworkReplicator::SpawnObject(NewPlayer);
        for (int i = 0; i < NewPlayer->Scripts.Count(); ++i)
        {
            NetworkReplicator::SpawnObject(NewPlayer->Scripts[i]);
        }
        NetworkReplicator::SetObjectOwnership(NewPlayer, client->ClientId, NetworkObjectRole::ReplicatedSimulated, true);
    }
    else
    {
        NetworkReplicator::SpawnObject(NewPlayer);
        for (int i = 0; i < NewPlayer->Scripts.Count(); ++i)
        {
            NetworkReplicator::SpawnObject(NewPlayer->Scripts[i]);
        }
        NetworkReplicator::SetObjectOwnership(NewPlayer, client->ClientId, NetworkObjectRole::OwnedAuthoritative, true);
        NewPlayer->GetScript<PlayerNetworking>()->ClaimAuthority();
    }
    
    Level::SpawnActor(NewPlayer);
}

void PlayerRespawn::OnPlayerDisconnected(NetworkClient* client)
{
    if (!_Players.ContainsKey(client))
    {
        return;
    }
    NetworkReplicator::DespawnObject(_Players[client]);
    _Players.Remove(client);
}
