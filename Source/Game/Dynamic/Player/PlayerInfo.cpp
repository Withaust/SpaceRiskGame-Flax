#include "PlayerInfo.h"

PlayerInfo::PlayerInfo(const SpawnParams& params)
    : Script(params)
{
}

void PlayerInfo::OnNetworkSpawn()
{
    UNOT_OWNED_RETURN();
    Name = Steam::Get()->GetPersonaName();
    Level = 1;
    Skill1 = 2;
    Skill2 = 3;
    TemplateId = 4;
}
