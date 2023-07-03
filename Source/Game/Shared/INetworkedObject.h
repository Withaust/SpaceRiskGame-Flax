#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/NetworkRpc.h>
#include <Engine/Networking/NetworkReplicator.h>

API_CLASS() class GAME_API INetworkedObject : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(INetworkedObject);

public:

    API_ENUM() enum class NetworkedType
    {
        None,
        Continuous,
        SpawnOnly,
    };

    API_FIELD() NetworkedType Networked = NetworkedType::None;
    API_FUNCTION(NetworkRpc = "Client, Reliable") void SendData(Array<byte> data, NetworkRpcParams info);
};
