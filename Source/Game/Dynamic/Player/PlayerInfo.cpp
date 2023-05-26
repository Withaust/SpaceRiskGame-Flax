#include "PlayerInfo.h"

PlayerInfo::PlayerInfo(const SpawnParams& params)
    : Script(params)
{
}

void PlayerInfo::OnNetworkSpawn()
{
    UNOT_OWNED_RETURN();
    Id = NetworkManager::LocalClientId;
    Name = Steam::GetPersonaName();
    Level = 1;
    Skill1 = 2;
    Skill2 = 3;
    TemplateId = 4;
    PlayerManager::Register(Id, GetActor());
}

void PlayerInfo::OnNetworkDespawn()
{
    PlayerManager::Unregister(Id);
}

void PlayerInfo::OnNetworkSync()
{
    PlayerManager::Register(Id, GetActor());
}
