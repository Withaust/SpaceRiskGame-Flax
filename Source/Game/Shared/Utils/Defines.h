#pragma once
#pragma warning (disable: 4003)

#include <Game/System/Core.h>
#include <Game/System/Core/Logger.h>
#include <Game/System/Core/Networking.h>
#include <Game/Shared/Utils/SleepBlock.h>
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
#define UPRINT_STR(text) Core::Get<Logger>()->Print(TEXT(text), __FILE__, __LINE__)
#define UPRINT(text, ...) Core::Get<Logger>()->Print(String::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)
#define UINFO_STR(text) Core::Get<Logger>()->Info(TEXT(text), __FILE__, __LINE__)
#define UINFO(text, ...) Core::Get<Logger>()->Info(String::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)
#else
#define UPRINT_STR(text)
#define UPRINT(text, ...)
#define UINFO_STR(text)
#define UINFO(text, ...)
#endif

#define UWARN_STR(text) Core::Get<Logger>()->Warning(TEXT(text), __FILE__, __LINE__)
#define UWARN(text, ...) Core::Get<Logger>()->Warning(String::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)

#define UERR_STR(text) Core::Get<Logger>()->Error(TEXT(text), __FILE__, __LINE__)
#define UERR(text, ...) Core::Get<Logger>()->Error(String::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)

#define UCRIT_STR(shutdown, text) Core::Get<Logger>()->Critical(shutdown, TEXT(text), __FILE__, __LINE__)
#define UCRIT(shutdown, text, ...) Core::Get<Logger>()->Critical(shutdown, Stringi::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)

// Polls target SleepGroup, and runs code block below as necessary
#define USLEEP(sleepBlock) if(sleepBlock.Poll(Time::GetDeltaTime()))

// Allows for the code to run in play mode only
#if USE_EDITOR
#define UNO_EDITOR(returnValue) if (!Editor::IsPlayMode) { return returnValue; }
#else
#define UNO_EDITOR(returnValue)
#endif
