#pragma once

#include <Game/System/CoreInstance.h>
#include <Game/System/Core/Logger.h>
#include <Game/Shared/Utils/SleepBlock.h>

// Declares singleton
// NOTE: LaunchArgs, Steam, Analytics, Logger cant use this file to define singleton and has to use manual declaration,
// since this file includes highest dependency, which is Logger, which in turn is going to make a recursive include loop
#define USINGLETON(Type) \
public: \
    FORCE_INLINE static Type* Get() { return CoreInstance::Instance()->Get<Type>(); } \
private: 

// Checks if current object is owned, if it is, then run code in the block
#define UOWNED if(NetworkReplicator::IsObjectOwned(this))
#define UOWNED_RETURN(returnValue) if(NetworkReplicator::IsObjectOwned(this)) { return returnValue; }
#define UOWNED_RETURN if(NetworkReplicator::IsObjectOwned(this)) { return; }

// Checks if current object is not owned, if it is, then run code in the block
#define UNOT_OWNED if(!NetworkReplicator::IsObjectOwned(this))
#define UNOT_OWNED_RETURN(returnValue) if(!NetworkReplicator::IsObjectOwned(this)) { return returnValue; }
#define UNOT_OWNED_RETURN if(!NetworkReplicator::IsObjectOwned(this)) { return; }

#ifdef BUILD_DEBUG
#define UPRINT_STR(text) Logger::Get()->Print(TEXT(text), __FILE__, __LINE__)
#define UPRINT(text, ...) Logger::Get()->Print(String::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)
#define UINFO_STR(text) Logger::Get()->Info(TEXT(text), __FILE__, __LINE__)
#define UINFO(text, ...) Logger::Get()->Info(String::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)
#else
#define UPRINT_STR(text)
#define UPRINT(text, ...)
#define UINFO_STR(text)
#define UINFO(text, ...)
#endif

#define UWARN_STR(text) Logger::Get()->Warning(TEXT(text), __FILE__, __LINE__)
#define UWARN(text, ...) Logger::Get()->Warning(String::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)

#define UERR_STR(text) Logger::Get()->Error(TEXT(text), __FILE__, __LINE__)
#define UERR(text, ...) Logger::Get()->Error(String::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)

#define UCRIT_STR(shutdown, text) Logger::Get()->Critical(shutdown, TEXT(text), __FILE__, __LINE__)
#define UCRIT(shutdown, text, ...) Logger::Get()->Critical(shutdown, Stringi::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)

// Polls target SleepGroup, and runs code block below as necessary
#define USLEEP(sleepBlock) if(sleepBlock.Poll(Time::GetDeltaTime()))
