#include "PostFx.h"

UIMPL_SINGLETON(PostFx);

PostFx::PostFx(const SpawnParams& params)
    : ISystem(params)
{
}

void PostFx::OnInitialize()
{
}

void PostFx::OnDeinitialize()
{
    UDEINIT_SINGLETON();
}