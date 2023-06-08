#include "PlayerInfo.h"

PlayerInfo::PlayerInfo(const SpawnParams& params)
    : Script(params)
{
}

void PlayerInfo::OnNetworkSpawn()
{
    UNOT_OWNED_RETURN();
    Id = NetworkManager::LocalClientId;
    Name = Core::Get<Steam>()->GetPersonaName();
    Level = 1;
    Skill1 = 2;
    Skill2 = 3;
    TemplateId = 4;
    Core::Get<PlayerManager>()->Register(Id, GetActor());
}

void PlayerInfo::OnNetworkDespawn()
{
    Core::Get<PlayerManager>()->Unregister(Id);
}

void PlayerInfo::OnNetworkSync()
{
    Core::Get<PlayerManager>()->Register(Id, GetActor());
}
