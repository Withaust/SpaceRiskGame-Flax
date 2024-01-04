#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/Shared/Defines.h>
#include <Game/System/Core/Networking.h>

API_CLASS() class GAME_API SyncInfo : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(SyncInfo);

private:

    enum SyncState
    {
        NotConnected,
        Connected
    };

    SyncState _syncState = SyncState::NotConnected;
    bool _askToSync = false;
    SleepBlock _syncBlock;
    HashSet<uint32> _syncList;

public:

    API_FIELD() static SyncInfo* Instance;

    void OnInitialize() override;
    void OnDeinitialize() override;
    void OnPlayerConnected(NetworkClient* client) override;
    void OnPlayerDisconnected(NetworkClient* client) override;
    void OnUpdate() override;

    API_FUNCTION(NetworkRpc = "Server, ReliableOrdered") void SendInfo(NetworkRpcParams param = NetworkRpcParams());
    API_FUNCTION(NetworkRpc = "Client, ReliableOrdered") void RecievedInfo(NetworkRpcParams param = NetworkRpcParams());
    void RequestSpawnSync();
};
