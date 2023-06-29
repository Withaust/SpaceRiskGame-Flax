#include "ISpawnSync.h"
#include <Game/System/Core/Networking/Networking.h>

ISpawnSync::ISpawnSync(const SpawnParams& params)
    : Script(params)
{
}

void ISpawnSync::SendData(NetworkRpcParams info, Array<byte> data)
{
    NETWORK_RPC_IMPL(ISpawnSync, SendData, info, data);

    NetworkStream* stream = Networking::Instance->_stream;

    stream->Initialize(data.begin(), data.Count());

    NetworkReplicator::InvokeSerializer(GetTypeHandle(), this, stream, false);
}
