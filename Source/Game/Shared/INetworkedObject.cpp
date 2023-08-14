#include "INetworkedObject.h"
#include <Game/System/Core/Networking.h>
#define LZ4_STATIC_LINKING_ONLY
#define LZ4_HC_STATIC_LINKING_ONLY
#include <Game/Thirdparty/lz4/lz4.h>
#include <Game/Thirdparty/lz4/lz4hc.h>

INetworkedObject::INetworkedObject(const SpawnParams& params)
    : Script(params)
{
}

void INetworkedObject::SendData(Array<char> data, int srcSize, NetworkRpcParams info)
{
    NETWORK_RPC_IMPL(INetworkedObject, SendData, data, srcSize, info);

    Networking::Instance->_buffer.Resize(srcSize);

    const int decompressed_size = LZ4_decompress_safe(&data[0], &Networking::Instance->_buffer[0], data.Count(), srcSize);

    if (decompressed_size < 0)
    {
        UCRIT(true, "A negative result from LZ4_decompress_safe indicates a failure trying to decompress the data.");
        return;
    }
    else if (decompressed_size != srcSize)
    {
        UCRIT(true, "LZ4_decompress_safe decompressed data is different from original!");
        return;
    }

    NetworkStream* stream = Networking::Instance->_stream;

    stream->Initialize((unsigned char*)Networking::Instance->_buffer.begin(), Networking::Instance->_buffer.Count());

    NetworkReplicator::InvokeSerializer(GetTypeHandle(), this, stream, false);

    if (INetworkObject* netObj = ScriptingObject::ToInterface<INetworkObject>(this))
    {
        netObj->OnNetworkSync();
    }
}
