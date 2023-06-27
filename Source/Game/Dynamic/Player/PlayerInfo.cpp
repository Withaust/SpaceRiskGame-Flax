#include "PlayerInfo.h"

PlayerInfo::PlayerInfo(const SpawnParams& params)
    : IComponent(params)
{
}

void PlayerInfo::OnNetworkSpawn()
{
    PlayerManager::Instance->Register(NetworkReplicator::GetObjectOwnerClientId(this), GetEntity());
    UNOT_OWNED_RETURN();
    Id = NetworkReplicator::GetObjectOwnerClientId(this);
    Name = Steam::Instance->GetPersonaName();
    Level = 1;
    Skill1 = 2;
    Skill2 = 3;
    TemplateId = 4;
}

void PlayerInfo::OnNetworkDespawn()
{
    PlayerManager::Instance->Unregister(NetworkReplicator::GetObjectOwnerClientId(this));
}
