#include "IDamagePenetratable.h"

IDamagePenetratable::IDamagePenetratable(const SpawnParams& params)
    : Script(params)
{
}

float IDamagePenetratable::InflictPenetration(Entity* Inflictor, float Damage)
{
    return Damage * DamageReduction;
}
