#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/NetworkRpc.h>
#include <Engine/Networking/NetworkManager.h>

#include <Game/Shared/Utils/Defines.h>

API_CLASS() class GAME_API Chat : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Chat);

    SleepBlock Event;

    void OnUpdate() override;

    API_FUNCTION(NetworkRpc = "Server, Reliable") void SendMessage(const String& text);
    API_FUNCTION(NetworkRpc = "Client, Reliable") void RecieveMessage(uint32 sender, const String& text);

};
