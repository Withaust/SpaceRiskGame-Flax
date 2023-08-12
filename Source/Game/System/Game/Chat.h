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

    API_FUNCTION(NetworkRpc = "Server, Reliable") void SendMessage(const String& text, NetworkRpcParams info = NetworkRpcParams());
    API_FUNCTION(NetworkRpc = "Client, Reliable") void RecieveMessage(uint32 sender, const String& text);

};
