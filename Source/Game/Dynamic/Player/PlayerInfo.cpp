#include "PlayerInfo.h"

PlayerInfo::PlayerInfo(const SpawnParams& params)
    : INetworked(params)
{
}

void PlayerInfo::OnSpawn()
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

void PlayerInfo::OnDespawn()
{
    Core::Get<PlayerManager>()->Unregister(Id);
}

void PlayerInfo::OnSync()
{
    if (!clientRegistered)
    {
        Core::Get<PlayerManager>()->Register(Id, GetActor());
        clientRegistered = true;
    }
}
