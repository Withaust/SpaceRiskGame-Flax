#include "IComponent.h"

IComponent::IComponent(const SpawnParams& params)
    : Script(params)
{
}

Entity* IComponent::GetEntity()
{
    if (!_entity)
    {
        _entity = Cast<Entity>(GetParent());
    }
    return _entity;
}

void IComponent::SendData(NetworkRpcParams info, Array<byte> data)
{
    NETWORK_RPC_IMPL(IComponent, SendData, info, data);
    
    NetworkStream* stream = Networking::Instance->_stream;

    stream->Initialize(data.begin(), data.Count());

    NetworkReplicator::InvokeSerializer(GetTypeHandle(), this, stream, false);
}
