#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/NetworkRpc.h>
#include <Engine/Networking/NetworkManager.h>

#include <Game/Shared/Defines.h>

API_CLASS(Namespace = "Game") class GAME_API Chat : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Chat);

    SleepBlock Event;

    void OnUpdate() override;

    API_EVENT() Delegate<uint32, String> OnChatMessage;

    API_FUNCTION(NetworkRpc = "Server, Reliable") void SendMessage(NetworkRpcParams info, const String& text);
    API_FUNCTION(NetworkRpc = "Client, Reliable") void RecieveMessage(uint32 sender, const String& text);

};
