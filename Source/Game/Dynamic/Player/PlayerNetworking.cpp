#include "PlayerNetworking.h"

PlayerNetworking::PlayerNetworking(const SpawnParams& params)
    : IComponent(params)
{
}

void PlayerNetworking::OnNetworkSpawn()
{
    UNOT_OWNED_RETURN();
    GetActor()->SetName(TEXT("Local Player"));
    Camera->SetIsActive(true);
    Direction->SetIsActive(false);
    PlayerManager::Instance->SetOurPlayer(GetActor());
}
