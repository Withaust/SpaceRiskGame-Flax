#pragma once

#include <Game/System/CoreInstance.h>
#include <Game/System/Core/Logger.h>
#include <Game/Utils/SleepBlock.h>

// Declares singleton
// NOTE: LaunchArgs, Steam, Analytics, Logger cant use this file to define singleton and has to use manual declaration,
// since this file includes highest dependency, which is Logger, which in turn is going to make a recursive include loop
#define USINGLETON(Type) \
public: \
    static Type* Get() { return CoreInstance::Instance()->Get<Type>(); } \
private: 

// Checks if current object is owned, if it is, then run code in the block
#define UOWNED(v) if(NetworkReplicator::IsObjectOwned(this))

// Checks if current object is not owned, if it is, then run code in the block
#define UNOT_OWNED(v) if(!NetworkReplicator::IsObjectOwned(this))

#ifdef BUILD_DEBUG
#define ULOG_DEBUG_STR(Text) Logger::Get()->Debug(TEXT(Text), __FILE__, __LINE__)
#define ULOG_DEBUG(Text, ...) Logger::Get()->Debug(String::Format(TEXT(Text), ##__VA_ARGS__), __FILE__, __LINE__)
#else
#define ULOG_DEBUG_STR(Text)
#define ULOG_DEBUG(Text, ...)
#endif

#define ULOG_INFO_STR(Text) Logger::Get()->Info(TEXT(Text), __FILE__, __LINE__)
#define ULOG_INFO(Text, ...) Logger::Get()->Info(String::Format(TEXT(Text), ##__VA_ARGS__), __FILE__, __LINE__)

#define ULOG_WARN_STR(Text) Logger::Get()->Warning(TEXT(Text), __FILE__, __LINE__)
#define ULOG_WARN(Text, ...) Logger::Get()->Warning(String::Format(TEXT(Text), ##__VA_ARGS__), __FILE__, __LINE__)

#define ULOG_ERR_STR(Text) Logger::Get()->Error(TEXT(Text), __FILE__, __LINE__)
#define ULOG_ERR(Text, ...) Logger::Get()->Error(String::Format(TEXT(Text), ##__VA_ARGS__), __FILE__, __LINE__)

#define ULOG_CRIT_STR(Text) Logger::Get()->Critical(TEXT(Text), __FILE__, __LINE__)
#define ULOG_CRIT(Text, ...) Logger::Get()->Critical(Stringi::Format(TEXT(Text), ##__VA_ARGS__), __FILE__, __LINE__)

// Polls target SleepGroup, and runs code block below as necessary
#define USLEEP(Name) if(Name.Poll(Time::GetDeltaTime()))
