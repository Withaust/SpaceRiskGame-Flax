#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/INetworkObject.h>

#include <Game/System/Core/Networking.h>

API_CLASS() class GAME_API PlayerOwned : public IComponent, public INetworkObject
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerOwned);

private:

    Guid _serverId = Guid::Empty;

public:

    API_PROPERTY(NetworkReplicated, Attributes = "NoSerialize, FlaxEngine.ReadOnly") Guid GetServerIdLocal() const { UNETPROP_GETLOCAL(_serverId); }
    API_PROPERTY(NetworkReplicated, Attributes = "NoSerialize, FlaxEngine.ReadOnly") void SetServerIdLocal(Guid value) { UNETPROP_SETLOCAL(_serverId); }
    API_FUNCTION(NetworkRpc = "Server, ReliableOrdered") void SetServerIdRemote(const Guid& value) { UNETPROP_SETREMOTE(PlayerOwned, ServerId); }
    API_FUNCTION(NetworkRpc = "Client, ReliableOrdered") void SetServerIdSync(const Guid& value, NetworkRpcParams p) { UNETPROP_SETSYNC(PlayerOwned, ServerId); }
};
