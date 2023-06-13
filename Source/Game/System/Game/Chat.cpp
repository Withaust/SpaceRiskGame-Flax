#include "Chat.h"

UIMPL_SINGLETON(Chat)

Chat::Chat(const SpawnParams& params)
    : ISystem(params)
{
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
