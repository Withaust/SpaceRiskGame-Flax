﻿#include "SyncInfo.h"

UIMPL_SINGLETON(SyncInfo);

SyncInfo::SyncInfo(const SpawnParams& params)
    : ISystem(params),
    _syncBlock(3.0f)
{
    _tickUpdate = true;
}

void SyncInfo::OnInitialize()
{

}

void SyncInfo::OnDeinitialize()
{

}

void SyncInfo::OnPlayerConnected(NetworkClient* client)
{
    _syncList.Add(client->ClientId);
}

void SyncInfo::OnPlayerDisconnected(NetworkClient* client)
{
    _syncList.Remove(client->ClientId);
}

void SyncInfo::OnUpdate()
{
    // TODO: https://github.com/FlaxEngine/FlaxEngine/issues/1211
    if (USLEEP(_syncBlock))
    {
        if (_askToSync)
        {
            SendInfo();
        }
    }
}

void SyncInfo::SendInfo(NetworkRpcParams param)
{
    NETWORK_RPC_IMPL(SyncInfo, SendInfo, param);
    if (!_syncList.Contains(param.SenderId))
    {
        return;
    }
    _syncList.Remove(param.SenderId);

    NetworkRpcParams params;
    uint32 ids[1] = { param.SenderId };
    params.TargetIds = ToSpan(ids, ARRAY_COUNT(ids));
    RecievedInfo(params);

    Networking::Instance->OnSynced(NetworkManager::GetClient(param.SenderId));
}

void SyncInfo::RecievedInfo(NetworkRpcParams param)
{
    NETWORK_RPC_IMPL(SyncInfo, RecievedInfo, param);
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
