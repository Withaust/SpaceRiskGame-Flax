#include "PlayerNetworking.h"

PlayerNetworking::PlayerNetworking(const SpawnParams& params)
    : IComponent(params)
{
}

void PlayerNetworking::OnNetworkSpawn()
{
    PlayerManager::Instance->Register(NetworkReplicator::GetObjectOwnerClientId(this), GetEntity());
    if(UOWNED)
    {
        Camera->SetIsActive(true);
        Direction->SetIsActive(false);
        GetEntity()->SetLayerRecursive(static_cast<int32>(Layers::PawnPlayerLocal));
        PlayerManager::Instance->SetOurPlayer(GetEntity());
        PostFx::Instance->Enable(GetEntity());
        GetEntity()->GetComponent<PawnInfo>()->SetNameRemote(Steam::Instance->GetPersonaName());
    }
}

void PlayerNetworking::OnNetworkDespawn()
{
    PlayerManager::Instance->Unregister(NetworkReplicator::GetObjectOwnerClientId(this));
}
