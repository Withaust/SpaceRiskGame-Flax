#include "Chat.h"

UIMPL_SINGLETON(Chat)

Chat::Chat(const SpawnParams& params)
    : ISystem(params)
{
    _tickUpdate = true;
}

void Chat::OnDeinitialize()
{
    UDEINIT_SINGLETON();
}

void Chat::SendMessage(const String& text, NetworkRpcParams info)
{
    NETWORK_RPC_IMPL(Chat, SendMessage, text, info);
    RecieveMessage(info.SenderId, text);
}

void Chat::RecieveMessage(uint32 sender, const String& text)
{
    NETWORK_RPC_IMPL(Chat, RecieveMessage, sender, text);
    OnChatMessage(sender, text);
}
