#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/NetworkRpc.h>
#include <Engine/Networking/NetworkReplicator.h>

API_CLASS() class GAME_API ISpawnSync : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(ISpawnSync);

public:
    API_FIELD() bool SyncOnlySpawn = false;
    API_FUNCTION(NetworkRpc = "Client, Reliable") void SendData(NetworkRpcParams info, Array<byte> data);
};
