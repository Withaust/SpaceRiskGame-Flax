#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/Shared/Defines.h>

API_CLASS() class GAME_API IDamagePenetratable : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(IDamagePenetratable);

public:

    API_FIELD() float DamageReduction = 0.9f;

    API_FUNCTION() float InflictPenetration(Entity* Inflictor, float Damage);

};
