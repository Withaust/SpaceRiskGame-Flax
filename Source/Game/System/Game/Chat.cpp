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
            SendMessage(TEXT("MESSAGE FROM CLIENT!"));
        }
    }
}

void Chat::SendMessage(const String& text)
{
    NETWORK_RPC_IMPL(Chat, SendMessage, text);
    RecieveMessage(NetworkManager::RPCInvoker, text);
}

void Chat::RecieveMessage(uint32 sender, const String& text)
{
    NETWORK_RPC_IMPL(Chat, RecieveMessage, sender, text);

    if (sender == NetworkManager::LocalClientId)
    {
        LOG(Info, "Got local call: {0}", text);
    }
    else
    {
        LOG(Info, "Got remote call: {0}", text);
    }
}
