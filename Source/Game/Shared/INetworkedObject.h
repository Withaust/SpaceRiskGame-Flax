﻿#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/NetworkRpc.h>
#include <Engine/Networking/NetworkReplicator.h>

API_CLASS() class GAME_API INetworkedObject : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(INetworkedObject);

public:

    API_ENUM() enum class NetworkingType
    {
        None,
        Registered,
        Replicated,
    };

    API_FIELD() NetworkingType Type = NetworkingType::None;
    API_FUNCTION(NetworkRpc = "Client, ReliableOrdered") void SendData(bool compressed, const Array<byte>& data, uint32 srcSize, NetworkRpcParams p);
};
