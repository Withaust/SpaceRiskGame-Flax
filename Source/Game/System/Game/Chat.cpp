#include "Chat.h"

Chat::Chat(const SpawnParams& params)
    : ISystem(params),
    Event(0.25f)
{
    _tickUpdate = true;
}

void Chat::OnUpdate()
{
    USLEEP(Event)
    {
        if (NetworkManager::IsConnected() && !NetworkManager::IsHost())
        {
            //SendMessage({}, TEXT("MESSAGE FROM CLIENT!"));
        }
    }
}

void Chat::SendMessage(NetworkRpcParams info, const String& text)
{
    NETWORK_RPC_IMPL(Chat, SendMessage, info, text);
    RecieveMessage(info.SenderId, text);
}

void Chat::RecieveMessage(uint32 sender, const String& text)
{
    NETWORK_RPC_IMPL(Chat, RecieveMessage, sender, text);
    OnChatMessage(sender, text);
}
