#include "PlayerNetworking.h"

PlayerNetworking::PlayerNetworking(const SpawnParams& params)
    : Script(params)
{
    _tickUpdate = true;
}

void PlayerNetworking::ClaimAuthority()
{
    GetActor()->SetName(TEXT("Local Player"));
    Camera->SetIsActive(true);
}

void PlayerNetworking::OnNetworkSpawn()
{
    UNOT_OWNED_RETURN;
    ClaimAuthority();
}