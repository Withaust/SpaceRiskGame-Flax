#include "Steam.h"

Steam::Steam(const SpawnParams& params)
    : GameSystem(params)
{
    _tickUpdate = true;
}

// Callbacks

void Steam::SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* callback)
{
    SteamNetConnectionStatusChanged(callback);
}

// SteamAPI

bool Steam::Init()
{
    return SteamAPI_Init();
}

void Steam::RunCallbacks()
{
    SteamAPI_RunCallbacks();
}

void Steam::Shutdown()
{
    SteamAPI_Shutdown();
}

bool Steam::RestartAppIfNecessary(uint32 unOwnAppID)
{
    return SteamAPI_RestartAppIfNecessary(unOwnAppID);
}

void Steam::ReleaseCurrentThreadMemory()
{
    SteamAPI_ReleaseCurrentThreadMemory();
}
// ISteamApps Interface
// ISteamAppTicket Interface
// ISteamClient Interface
// ISteamFriends Interface
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

HSteamListenSocket Steam::CreateListenSocketP2P(int nVirtualPort, int nOptions, const SteamNetworkingConfigValue_t* pOptions)
{
    if (!_initialized)
    {
        return 0;
    }
    return SteamNetworkingSockets()->CreateListenSocketP2P(nVirtualPort, nOptions, pOptions);
}

HSteamNetConnection Steam::ConnectP2P(const SteamNetworkingIdentity& identityRemote, int nVirtualPort, int nOptions, const SteamNetworkingConfigValue_t* pOptions)
{
    if (!_initialized)
    {
        return 0;
    }
    return SteamNetworkingSockets()->ConnectP2P(identityRemote, nVirtualPort, nOptions, pOptions);
}

EResult Steam::AcceptConnection(HSteamNetConnection hConn)
{
    if (!_initialized)
    {
        return k_EResultDisabled;
    }
    return SteamNetworkingSockets()->AcceptConnection(hConn);
}

bool Steam::CloseConnection(HSteamNetConnection hPeer, int nReason, const char* pszDebug, bool bEnableLinger)
{
    if (!_initialized)
    {
        return false;
    }
    return SteamNetworkingSockets()->CloseConnection(hPeer, nReason, pszDebug, bEnableLinger);
}

bool Steam::CloseListenSocket(HSteamListenSocket hSocket)
{
    if (!_initialized)
    {
        return false;
    }
    return SteamNetworkingSockets()->CloseListenSocket(hSocket);
}

EResult Steam::SendMessageToConnection(HSteamNetConnection hConn, const void* pData, uint32 cbData, int nSendFlags, int64* pOutMessageNumber)
{
    if (!_initialized)
    {
        return k_EResultDisabled;
    }
    return SteamNetworkingSockets()->SendMessageToConnection(hConn, pData, cbData, nSendFlags, pOutMessageNumber);
}

int Steam::ReceiveMessagesOnConnection(HSteamNetConnection hConn, SteamNetworkingMessage_t** ppOutMessages, int nMaxMessages)
{
    if (!_initialized)
    {
        return 0;
    }
    return SteamNetworkingSockets()->ReceiveMessagesOnConnection(hConn, ppOutMessages, nMaxMessages);
}

HSteamNetPollGroup Steam::CreatePollGroup()
{
    if (!_initialized)
    {
        return 0;
    }
    return SteamNetworkingSockets()->CreatePollGroup();
}

bool Steam::DestroyPollGroup(HSteamNetPollGroup hPollGroup)
{
    if (!_initialized)
    {
        return false;
    }
    return SteamNetworkingSockets()->DestroyPollGroup(hPollGroup);
}

bool Steam::SetConnectionPollGroup(HSteamNetConnection hConn, HSteamNetPollGroup hPollGroup)
{
    if (!_initialized)
    {
        return false;
    }
    return SteamNetworkingSockets()->SetConnectionPollGroup(hConn, hPollGroup);
}

int Steam::ReceiveMessagesOnPollGroup(HSteamNetPollGroup hPollGroup, SteamNetworkingMessage_t** ppOutMessages, int nMaxMessages)
{
    if (!_initialized)
    {
        return 0;
    }
    return SteamNetworkingSockets()->ReceiveMessagesOnPollGroup(hPollGroup, ppOutMessages, nMaxMessages);
}

// ISteamNetworkingUtils Interface

void Steam::InitRelayNetworkAccess()
{
    if (!_initialized)
    {
        return;
    }
    SteamNetworkingUtils()->InitRelayNetworkAccess();
}

// ISteamParties Interface
// ISteamRemotePlay Interface
// ISteamRemoteStorage Interface
// ISteamScreenshots Interface
// ISteamUGC Interface

// ISteamUser Interface

CSteamID Steam::GetSteamID()
{
    if (!_initialized)
    {
        return CSteamID();
    }
    return SteamUser()->GetSteamID();
}

uint64_t Steam::GetSteamID64()
{
    if (!_initialized)
    {
        return 0;
    }
    return SteamUser()->GetSteamID().ConvertToUint64();
}

uint64_t Steam::GetSteamID64(CSteamID SteamID)
{
    if (!_initialized)
    {
        return 0;
    }
    return SteamID.ConvertToUint64();
}

// ISteamUserStats Interface
// ISteamUtils Interface
// ISteamVideo Interface

void Steam::OnInitialize()
{
    if (RestartAppIfNecessary(_appID))
    {
        DebugLog::Log(TEXT("Steam asked us to restart an app."));
        Engine::RequestExit(0);
        return;
    }

    _initialized = Init();

    if (!_initialized)
    {
        String ErrorText = TEXT("Failed to initialize Steam. Try restarting the app and/or Steam.");
        DebugLog::LogError(ErrorText);
        Platform::Error(ErrorText);
        Engine::RequestExit(1);
        return;
    }

    InitRelayNetworkAccess();
}

void Steam::OnDeinitialize()
{
    if (_initialized)
    {
        Shutdown();
    }
}

void Steam::OnUpdate()
{
    if (_initialized)
    {
        RunCallbacks();
    }
}
