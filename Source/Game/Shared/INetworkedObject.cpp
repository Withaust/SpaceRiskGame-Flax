#include "INetworkedObject.h"
#include <Game/System/Core/Networking.h>

INetworkedObject::INetworkedObject(const SpawnParams& params)
    : Script(params)
{
}

void INetworkedObject::SendData(bool compressed, const Array<byte>& data, uint32 srcSize, NetworkRpcParams info)
{
    NETWORK_RPC_IMPL(INetworkedObject, SendData, compressed, data, srcSize, info);

    NetworkStream* stream = Networking::Instance->_stream;

    if (compressed && srcSize != 0)
    {
        if (!EngineHelper::Decompress(data, srcSize))
        {
            UCRIT(true, "EngineHelper::Decompress failed to decompress a replication data.");
            return;
        }

        const auto& buffer = EngineHelper::GetCompressBuffer();

        stream->Initialize((byte*)buffer.begin(), buffer.Count());
    }
    else
    {
        stream->Initialize((byte*)data.begin(), data.Count());
    }

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
