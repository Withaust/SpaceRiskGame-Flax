#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/Shared/Defines.h>
#include <Game/System/Core/Networking/Networking.h>
#include <Game/System/Core/Networking/CustomHierarchy.h>

API_CLASS() class GAME_API SyncInfo : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(SyncInfo);

private:

    bool _askingForSync = false;
    SleepBlock _syncBlock;
    HashSet<uint32> _syncList;

public:

    API_FIELD() static SyncInfo* Instance;

    void OnInitialize() override;
    void OnDeinitialize() override;
    void OnPlayerConnected(NetworkClient* client) override;
    void OnPlayerDisconnected(NetworkClient* client) override;
    void OnUpdate() override;

    API_FUNCTION(NetworkRpc = "Server, Reliable") void SendInfo(NetworkRpcParams param = NetworkRpcParams());
    API_FUNCTION(NetworkRpc = "Client, Reliable") void RecievedInfo(NetworkRpcParams param = NetworkRpcParams());
    void RequestSpawnSync();
};
