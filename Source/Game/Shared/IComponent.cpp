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

// TODO: https://github.com/FlaxEngine/FlaxEngine/issues/1209
void IComponent::SendData(NetworkRpcParams info, Variant data)
{
    NETWORK_RPC_IMPL(IComponent, SendData, info, data);
    
    NetworkStream* stream = Networking::Instance->_stream;

    stream->Initialize(static_cast<byte*>(data.AsBlob.Data), data.AsBlob.Length);

    NetworkReplicator::InvokeSerializer(GetTypeHandle(), this, stream, false);
}
