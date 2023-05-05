#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/INetworkObject.h>
#include <Engine/Networking/INetworkSerializable.h>

API_CLASS() class GAME_API INetworked : public Script, public INetworkObject
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(INetworked);

private:
    bool hasSynced = false;
public:

    API_FUNCTION() virtual void OnSpawn() {};
    API_FUNCTION() virtual void OnDespawn() {};
    API_FUNCTION() virtual void OnSync() {};

    void OnNetworkSpawn() override { OnSpawn(); };
    void OnNetworkDespawn() override { OnDespawn(); };
    void OnNetworkSerialize() override {};
    void OnNetworkDeserialize() override
    {
        if (!hasSynced)
        {
            OnSync();
            hasSynced = true;
        }
    };
};
