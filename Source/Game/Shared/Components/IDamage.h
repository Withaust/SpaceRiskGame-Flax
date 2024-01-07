#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/INetworkObject.h>

#include <Game/Shared/Defines.h>
#include <Game/System/Core/Networking.h>
#include <Game/Shared/Components/IHitBox.h>
#include <Game/Shared/Components/SpatialSync.h>

API_CLASS() class GAME_API IDamage : public IComponent, public INetworkObject
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(IDamage);
    friend class IHitBox;

private:

    Array<IHitBox*> _hitboxes;

public:

    API_EVENT() Delegate<uint32, Entity*, float> OnDamage;
    API_EVENT() Delegate<uint32, Entity*, float> OnDestroyed;

    API_FUNCTION(NetworkRpc = "Server, ReliableOrdered") void InflictDamageServer(uint32 HitBox, Guid Inflictor, float Damage);
    API_FUNCTION(NetworkRpc = "Client, ReliableOrdered") void InflictDamageClient(uint32 HitBox, Guid Inflictor, float Damage, NetworkRpcParams p);

    void OnNetworkSpawn() override;
    void OnNetworkDespawn() override;
};
