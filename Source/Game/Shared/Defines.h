#pragma once
#pragma warning (disable: 4003)

#include <Game/System/Core.h>
#include <Game/System/Core/Logger.h>
#include <Game/Shared/Entity.h>
#include <Engine/Content/JsonAsset.h>
#include <Game/Shared/SleepBlock.h>
#include <Game/Shared/MathHelper.h>
#include <Game/Shared/EngineHelper.h>
#include <Game/System/Core/Networking.h>
#if USE_EDITOR
#include <Editor/Editor.h>
#include <Editor/Managed/ManagedEditor.h>
#endif

// Singleton macro implementation
#define UIMPL_SINGLETON(T) T* T::Instance = nullptr;
#define UDEINIT_SINGLETON() Instance = nullptr;

// Checks if current object is owned, if it is, then run code in the block
#define UOWNED NetworkReplicator::IsObjectOwned(GetEntity())
#define UOWNED_RETURN(returnValue) if(NetworkReplicator::IsObjectOwned(GetEntity())) { return returnValue; }

// Checks if current object is not owned, if it is, then run code in the block
#define UNOT_OWNED !NetworkReplicator::IsObjectOwned(GetEntity())
#define UNOT_OWNED_RETURN(returnValue) if(!NetworkReplicator::IsObjectOwned(GetEntity())) { return returnValue; }

// Implements filtering for RPC calls that should never arrive to the clients that arent synced yet
#define UFILTER_RPC(params) \
NetworkRpcParams params; \
Array<uint32> targets = { 0 }; \
if (SyncInfo::Instance && SyncInfo::Instance->GetSyncList().Count() > 0) { \
    for (const auto& client : NetworkManager::Clients) { \
        if (!SyncInfo::Instance->GetSyncList().Contains(client->ClientId)) { \
            targets.Add(client->ClientId); } } } \
else { \
    for (const auto& client : NetworkManager::Clients) { \
        targets.Add(client->ClientId); } } \
params.TargetIds = ToSpan(targets.Get(), targets.Count());

// Implements generic networked property for a component
#define UNETPROP_GETLOCAL(PrivateValue) return PrivateValue;
#define UNETPROP_SETLOCAL(PrivateValue) PrivateValue = value;
#define UNETPROP_SETREMOTE(Class, ValueName) NETWORK_RPC_IMPL(Class, Set##ValueName##Remote, value); UFILTER_RPC(params); Set##ValueName##Sync(value, params);
#define UNETPROP_SETSYNC(Class, ValueName) NETWORK_RPC_IMPL(Class, Set##ValueName##Sync, value, p); Set##ValueName##Local(value);
#define UNETPROP_SETSYNC_DATA(Class, ValueName) NETWORK_RPC_IMPL(Class, Set##ValueName##Sync, value, p); ValueName = value;

#ifdef BUILD_DEBUG
#define UINFO(text, ...) Logger::Instance->Info(String::Format(TEXT(text), ##__VA_ARGS__))
#define UINFO_STR(string) Logger::Instance->Info(string)
#else
#define UINFO(text, ...)
#define UINFO_STR(string)
#endif

#define UWARN(text, ...) Logger::Instance->Warning(String::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)
#define UWARN_STR(string) Logger::Instance->Warning(string, __FILE__, __LINE__)
#define UERR(text, ...) Logger::Instance->Error(String::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)
#define UERR_STR(string) Logger::Instance->Error(string, __FILE__, __LINE__)
#define UCRIT(shutdown, text, ...) Logger::Instance->Critical(shutdown, String::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)
#define UCRIT_STR(shutdown, string) Logger::Instance->Critical(shutdown, string, __FILE__, __LINE__)

// Polls target SleepGroup, and runs code block below as necessary
#define USLEEP(sleepBlock) sleepBlock.Poll(Time::GetDeltaTime())

// Allows for the code to run in play mode only
#if USE_EDITOR
#define UNO_EDITOR(returnValue) if (!Editor::IsPlayMode) { return returnValue; }
#else
#define UNO_EDITOR(returnValue)
#endif

// Declares generic Data member for a component
#define UDECLARE_DATA(DataType, Name) \
DataType* Name##Ptr = nullptr; \
void On##Name##Changed() { \
    DataType* result = Name->GetInstance<DataType>(); \
    if (!result) { \
    UERR("Data field {0} failed to filter datatype assignment for {1}", TEXT(#Name), TEXT(#DataType)); \
    } Name##Ptr = result; } \

#define UBIND_DATA(Class, Name) Name.Changed.Bind<Class, &Class::On##Name##Changed>(this); \
if(!Name) { UERR("Data field {0} is empty for object {1}", TEXT(#Name), GetEntity()->GetName()); } \
if (Name->WaitForLoaded()) { \
    UERR("Data field {0} is failed to load on object {1}", TEXT(#Name), GetEntity()->GetName()); return; \
} On##Name##Changed();

// Struct serialization related stuff
#define UDESERIALIZE_MEMBER(member) DESERIALIZE_MEMBER(member, v.member)
#define USERIALIZE_MEMBER(member) stream.JKEY(#member); Serialize(stream, v.member, nullptr);
