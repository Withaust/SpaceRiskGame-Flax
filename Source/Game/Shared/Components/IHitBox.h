#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/NetworkReplicator.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/Components/IDamage.h>

API_CLASS() class GAME_API IHitBox : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(IHitBox);
    friend class IDamage;

public:

    API_FIELD() float DamageMultiplier = 1.0f;
    API_FIELD() float Health = 100.0f;
    API_FIELD() uint32 Index = 0;
};
