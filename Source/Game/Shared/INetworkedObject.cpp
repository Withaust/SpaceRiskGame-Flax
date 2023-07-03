#include "INetworkedObject.h"
#include <Game/System/Core/Networking/Networking.h>

INetworkedObject::INetworkedObject(const SpawnParams& params)
    : Script(params)
{
}

void INetworkedObject::SendData(Array<byte> data, NetworkRpcParams info)
{
    NETWORK_RPC_IMPL(INetworkedObject, SendData, data, info);

    NetworkStream* stream = Networking::Instance->_stream;

    stream->Initialize(data.begin(), data.Count());

    NetworkReplicator::InvokeSerializer(GetTypeHandle(), this, stream, false);
}
