#pragma once
#pragma warning (disable: 4003)

#include <Game/System/Core.h>
#include <Game/System/Core/Logger.h>
#include <Game/System/Core/Networking.h>
#include <Game/Shared/SleepBlock.h>
#if USE_EDITOR
#include <Editor/Editor.h>
#include <Editor/Managed/ManagedEditor.h>
#endif

// Checks if current object is owned, if it is, then run code in the block
#define UOWNED if(NetworkReplicator::IsObjectOwned(this))
#define UOWNED_RETURN(returnValue) if(NetworkReplicator::IsObjectOwned(this)) { return returnValue; }

// Checks if current object is not owned, if it is, then run code in the block
#define UNOT_OWNED if(!NetworkReplicator::IsObjectOwned(this))
#define UNOT_OWNED_RETURN(returnValue) if(!NetworkReplicator::IsObjectOwned(this)) { return returnValue; }

#ifdef BUILD_DEBUG
#define UPRINT_STR(text) Logger::Print(TEXT(text))
#define UPRINT(text, ...) Logger::Print(String::Format(TEXT(text), ##__VA_ARGS__))
#define UINFO_STR(text) Logger::Info(TEXT(text))
#define UINFO(text, ...) Logger::Info(String::Format(TEXT(text), ##__VA_ARGS__))
#else
#define UPRINT_STR(text)
#define UPRINT(text, ...)
#define UINFO_STR(text)
#define UINFO(text, ...)
#endif

#define UWARN_STR(text) Logger::Warning(TEXT(text), __FILE__, __LINE__)
#define UWARN(text, ...) Logger::Warning(String::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)

#define UERR_STR(text) Logger::Error(TEXT(text), __FILE__, __LINE__)
#define UERR(text, ...) Logger::Error(String::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)

#define UCRIT_STR(shutdown, text) Logger::Critical(shutdown, TEXT(text), __FILE__, __LINE__)
#define UCRIT(shutdown, text, ...) Logger::Critical(shutdown, Stringi::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)

// Polls target SleepGroup, and runs code block below as necessary
#define USLEEP(sleepBlock) if(sleepBlock.Poll(Time::GetDeltaTime()))

// Allows for the code to run in play mode only
#if USE_EDITOR
#define UNO_EDITOR(returnValue) if (!Editor::IsPlayMode) { return returnValue; }
#else
#define UNO_EDITOR(returnValue)
#endif

// Network RPC implementation (placed in the beginning of the method body)
#define NETWORK_RPC_SYSTEM_IMPL(type, name, ...) \
    { \
    const NetworkRpcInfo* rpcInfoPtr = NetworkRpcInfo::RPCsTable.TryGet(NetworkRpcName(type::TypeInitializer, StringAnsiView(#name))); \
    if (rpcInfoPtr == nullptr) \
    { \
        LOG(Error, "Invalid RPC {0}::{1}. Ensure to use proper type name and method name (and 'Network' tag on a code module).", TEXT(#type), TEXT(#name)); \
        if (Platform::IsDebuggerPresent()) \
            PLATFORM_DEBUG_BREAK; \
        Platform::Assert("Invalid RPC.", __FILE__, __LINE__); \
        return; \
    } \
    const NetworkRpcInfo& rpcInfo = *rpcInfoPtr; \
    const NetworkManagerMode networkMode = NetworkManager::Mode; \
    if ((rpcInfo.Server && networkMode == NetworkManagerMode::Client) || (rpcInfo.Client && networkMode != NetworkManagerMode::Client)) \
    { \
        Array<void*, FixedAllocation<16>> args; \
        NetworkRpcInitArg(args, __VA_ARGS__); \
        rpcInfo.Invoke(Core::Get<type>(), args.Get()); \
        if (rpcInfo.Server && networkMode == NetworkManagerMode::Client) \
            return; \
        if (rpcInfo.Client && networkMode == NetworkManagerMode::Server) \
            return; \
    } \
    }
