#include "PlayerInfo.h"

PlayerInfo::PlayerInfo(const SpawnParams& params)
    : IComponent(params)
{
}

void PlayerInfo::OnNetworkSpawn()
{
    UNOT_OWNED_RETURN();
    Id = NetworkManager::LocalClientId;
    Name = Steam::Instance->GetPersonaName();
    Level = 1;
    Skill1 = 2;
    Skill2 = 3;
    TemplateId = 4;
    PlayerManager::Instance->Register(Id, GetActor());
}

void PlayerInfo::OnNetworkDespawn()
{
    PlayerManager::Instance->Unregister(Id);
}

void PlayerInfo::OnNetworkSync()
{
    PlayerManager::Instance->Register(Id, GetActor());
}
