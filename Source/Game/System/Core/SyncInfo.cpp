#include "SyncInfo.h"

UIMPL_SINGLETON(SyncInfo);

SyncInfo::SyncInfo(const SpawnParams& params)
    : ISystem(params)
{
    _tickUpdate = true;
}

void SyncInfo::OnInitialize()
{
    _syncBlock = 3.0f;
}

void SyncInfo::OnDeinitialize()
{
    UDEINIT_SINGLETON();
}

void SyncInfo::OnPlayerConnected(NetworkClient* client)
{
    if (client->ClientId != 0)
    {
        _syncList.Add(client->ClientId);
    }
}

void SyncInfo::OnPlayerDisconnected(NetworkClient* client)
{
    if (client->ClientId != 0)
    {
        _syncList.Remove(client->ClientId);
    }
}

void SyncInfo::OnUpdate()
{
    if (USLEEP(_syncBlock))
    {
        if (_askToSync)
        {
            SendInfo();
        }
    }
}

void SyncInfo::SendInfo(NetworkRpcParams p)
{
    NETWORK_RPC_IMPL(SyncInfo, SendInfo, p);
    if (!_syncList.Contains(p.SenderId))
    {
        return;
    }

    NetworkRpcParams params;
    uint32 ids[1] = { p.SenderId };
    params.TargetIds = ToSpan(ids, ARRAY_COUNT(ids));
    RecievedInfo(params);

    Networking::Instance->OnSynced(NetworkManager::GetClient(p.SenderId));

    _syncList.Remove(p.SenderId);
}

void SyncInfo::RecievedInfo(NetworkRpcParams p)
{
    NETWORK_RPC_IMPL(SyncInfo, RecievedInfo, p);
    _syncState = SyncState::Connected;
    _askToSync = false;
}

void SyncInfo::RequestSpawnSync()
{
    if (_syncState == SyncState::NotConnected)
    {
        _askToSync = true;
    }
}
