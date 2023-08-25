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

    API_PROPERTY(NetworkReplicated) Guid GetServerIdLocal() const { UIMPL_NETPROP_GETLOCAL(_serverId); }
    API_PROPERTY(NetworkReplicated) void SetServerIdLocal(Guid value) { UIMPL_NETPROP_SETLOCAL(_serverId); }
    API_FUNCTION(NetworkRpc = "Server, Reliable") void SetServerIdRemote(const Guid& value) { UIMPL_NETPROP_SETREMOTE(PlayerOwned, ServerId); }
    API_FUNCTION(NetworkRpc = "Client, Reliable") void SetServerIdSync(const Guid& value) { UIMPL_NETPROP_SETSYNC(PlayerOwned, ServerId); }
};
