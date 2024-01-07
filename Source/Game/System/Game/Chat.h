#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/NetworkRpc.h>
#include <Engine/Networking/NetworkManager.h>

#include <Game/Shared/Defines.h>

API_CLASS() class GAME_API Chat : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Chat);

public:
    API_FIELD() static Chat* Instance;

    API_EVENT() Delegate<uint32, String> OnChatMessage;

    void OnDeinitialize() override;

    API_FUNCTION(NetworkRpc = "Server, ReliableOrdered") void SendMessage(const String& text, NetworkRpcParams p = NetworkRpcParams());
    API_FUNCTION(NetworkRpc = "Client, ReliableOrdered") void RecieveMessage(uint32 sender, const String& text, NetworkRpcParams p);

};
