#include "KillFeed.h"

UIMPL_SINGLETON(KillFeed)

KillFeed::KillFeed(const SpawnParams& params)
    : ISystem(params)
{
}

void KillFeed::OnDeinitialize()
{
    UDEINIT_SINGLETON();
}