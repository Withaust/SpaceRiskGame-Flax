#include "PlayerInfo.h"

PlayerInfo::PlayerInfo(const SpawnParams& params)
    : Script(params)
{
}

void PlayerInfo::OnEnable()
{
    if (NetworkReplicator::IsObjectOwned(this))
    {
        UPRINT("OnEnable() -> Owned");
    }
    else
    {
        UPRINT("OnEnable() -> Not Owned");
    }
}

void PlayerInfo::OnStart()
{
    if (NetworkReplicator::IsObjectOwned(this))
    {
        UPRINT("OnStart() -> Owned");
    }
    else
    {
        UPRINT("OnStart() -> Not Owned");
    }
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
