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

void Chat::SendMessage(const String& text, NetworkRpcParams p)
{
    NETWORK_RPC_IMPL(Chat, SendMessage, text, p);
    UFILTER_RPC(params);
    RecieveMessage(p.SenderId, text, params);
}

void Chat::RecieveMessage(uint32 sender, const String& text, NetworkRpcParams p)
{
    NETWORK_RPC_IMPL(Chat, RecieveMessage, sender, text, p);
    OnChatMessage(sender, text);
}
