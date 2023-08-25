#include "INetworkedObject.h"
#include <Game/System/Core/Networking.h>

INetworkedObject::INetworkedObject(const SpawnParams& params)
    : Script(params)
{
}

void INetworkedObject::SendData(const Array<char>& data, int srcSize, NetworkRpcParams info)
{
    NETWORK_RPC_IMPL(INetworkedObject, SendData, data, srcSize, info);

    if (!EngineHelper::Decompress(data, srcSize))
    {
        UCRIT(true, "EngineHelper::Decompress failed to decompress a replication data.");
        return;
    }

    NetworkStream* stream = Networking::Instance->_stream;

    const auto& buffer = EngineHelper::GetCompressBuffer();

    stream->Initialize((unsigned char*)buffer.begin(), buffer.Count());

    if (INetworkSerializable* networked = ToInterface<INetworkSerializable>(this))
    {
        networked->Deserialize(stream);
    }
    else
    {
        NetworkReplicator::InvokeSerializer(GetTypeHandle(), this, stream, false);
    }

    if (INetworkObject* netObj = ScriptingObject::ToInterface<INetworkObject>(this))
    {
        netObj->OnNetworkSync();
    }
}
