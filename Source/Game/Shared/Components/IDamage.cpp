﻿#include "IDamage.h"

IDamage::IDamage(const SpawnParams& params)
    : IComponent(params)
{
}

void IDamage::OnNetworkSpawn()
{
    EngineHelper::FindScripts(GetEntity(), _hitboxes);

    for (int i = 0; i < _hitboxes.Count(); ++i)
    {
        auto hitbox = _hitboxes[i];
        hitbox->Index = i;
    }
}

void IDamage::OnNetworkDespawn()
{

}

void IDamage::InflictDamageServer(uint32 HitBox, Guid Inflictor, float Damage)
{
    NETWORK_RPC_IMPL(IDamage, InflictDamageServer, HitBox, Inflictor, Damage);

    if (Entity* entity = Networking::Instance->ConvertEntity(Inflictor))
    {
        UFILTER_RPC(params);
        InflictDamageClient(HitBox, entity->GetID(), Damage, params);
    }
    else
    {
        UERR("Failed to resolve entity!");
    }
}

void IDamage::InflictDamageClient(uint32 HitBox, Guid Inflictor, float Damage, NetworkRpcParams p)
{
    NETWORK_RPC_IMPL(IDamage, InflictDamageClient, HitBox, Inflictor, Damage, p);

    if (Entity* inflictor = Networking::Instance->ConvertEntity(Inflictor))
    {
        IHitBox* hitBox = _hitboxes[HitBox];

        if (hitBox->Health <= 0.0f)
        {
            return;
        }

        hitBox->Health -= Damage * hitBox->DamageMultiplier;
        OnDamage(HitBox, inflictor, Damage);
        if (hitBox->Health <= 0.0f)
        {
            OnDestroyed(HitBox, inflictor, Damage);
        }
    }
    else
    {
        UERR("Failed to resolve entity!");
    }
}
