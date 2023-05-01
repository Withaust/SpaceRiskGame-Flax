#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Debug/DebugLog.h>
#include <Engine/Engine/Engine.h>
#include <Engine/Core/RandomStream.h>

#include <SteamWorks/steam_api.h>

#include <Game/System/CoreInstance.h>
#include <Game/System/Core/LaunchArgs.h>

API_CLASS() class GAME_API Steam : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Steam);
    static Steam* Get() { return CoreInstance::Instance()->Get<Steam>(); }

private:

    bool _initialized = false;
    const int _appID = 1436420;
    String _name;
    CSteamID _steamID;

public:

    // Callbacks
    STEAM_CALLBACK(Steam, SteamNetConnectionStatusChangedCallback, SteamNetConnectionStatusChangedCallback_t);

    API_EVENT() Delegate<SteamNetConnectionStatusChangedCallback_t*> SteamNetConnectionStatusChanged;

    // SteamAPI
    bool Init();
    void RunCallbacks();
    void Shutdown();
    bool RestartAppIfNecessary(uint32 unOwnAppID);
    void ReleaseCurrentThreadMemory();
    // ISteamApps Interface
    // ISteamAppTicket Interface
    // ISteamClient Interface
    // ISteamFriends Interface
    String GetPersonaName();
    // ISteamGameCoordinator Interface
    // ISteamGameServer Interface
    // ISteamGameServerStats Interface
    // ISteamHTMLSurface Interface
    // ISteamHTTP Interface
    // ISteamInput Interface
    // ISteamInventory Interface
    // ISteamMatchmaking Interface
    // ISteamMatchmakingServers Interface
    // ISteamMusic Interface
    // ISteamMusicRemote Interface
    // ISteamNetworking Interface
    // ISteamNetworkingMessages Interface
    // ISteamNetworkingSockets Interface
    HSteamListenSocket CreateListenSocketP2P(int nVirtualPort, int nOptions, const SteamNetworkingConfigValue_t* pOptions);
    HSteamNetConnection ConnectP2P(const SteamNetworkingIdentity& identityRemote, int nVirtualPort, int nOptions, const SteamNetworkingConfigValue_t* pOptions);
    EResult AcceptConnection(HSteamNetConnection hConn);
    bool CloseConnection(HSteamNetConnection hPeer, int nReason, const char* pszDebug, bool bEnableLinger);
    bool CloseListenSocket(HSteamListenSocket hSocket);
    EResult SendMessageToConnection(HSteamNetConnection hConn, const void* pData, uint32 cbData, int nSendFlags, int64* pOutMessageNumber);
    int ReceiveMessagesOnConnection(HSteamNetConnection hConn, SteamNetworkingMessage_t** ppOutMessages, int nMaxMessages);
    HSteamNetPollGroup CreatePollGroup();
    bool DestroyPollGroup(HSteamNetPollGroup hPollGroup);
    bool SetConnectionPollGroup(HSteamNetConnection hConn, HSteamNetPollGroup hPollGroup);
    int ReceiveMessagesOnPollGroup(HSteamNetPollGroup hPollGroup, SteamNetworkingMessage_t** ppOutMessages, int nMaxMessages);
    // ISteamNetworkingUtils Interface
    void InitRelayNetworkAccess();
    // ISteamParties Interface
    // ISteamRemotePlay Interface
    // ISteamRemoteStorage Interface
    // ISteamScreenshots Interface
    // ISteamUGC Interface
    // ISteamUser Interface
    CSteamID GetSteamID();
    uint64_t GetSteamID64();
    uint64_t GetSteamID64(CSteamID SteamID);
    // ISteamUserStats Interface
    // ISteamUtils Interface
    // ISteamVideo Interface
    // Flax API
    void OnInitialize() override;
    void OnDeinitialize() override;
    void OnUpdate() override;
};
