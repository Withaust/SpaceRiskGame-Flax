#pragma once
#pragma warning (disable: 4003)

#include <Game/System/Core.h>
#include <Game/System/Core/Logger.h>
#include <Game/System/Core/Networking/Networking.h>
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
#define UPRINT_STR(text) Logger::Instance->Print(TEXT(text))
#define UPRINT(text, ...) Logger::Instance->Print(String::Format(TEXT(text), ##__VA_ARGS__))
#define UINFO_STR(text) Logger::Instance->Info(TEXT(text))
#define UINFO(text, ...) Logger::Instance->Info(String::Format(TEXT(text), ##__VA_ARGS__))
#else
#define UPRINT_STR(text)
#define UPRINT(text, ...)
#define UINFO_STR(text)
#define UINFO(text, ...)
#endif

#define UWARN_STR(text) Logger::Instance->Warning(TEXT(text), __FILE__, __LINE__)
#define UWARN(text, ...) CLogger::Instance->Warning(String::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)

#define UERR_STR(text) Logger::Instance->Error(TEXT(text), __FILE__, __LINE__)
#define UERR(text, ...) Logger::Instance->Error(String::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)

#define UCRIT_STR(shutdown, text) Logger::Instance->Critical(shutdown, TEXT(text), __FILE__, __LINE__)
#define UCRIT(shutdown, text, ...) Logger::Instance->Critical(shutdown, Stringi::Format(TEXT(text), ##__VA_ARGS__), __FILE__, __LINE__)

// Polls target SleepGroup, and runs code block below as necessary
#define USLEEP(sleepBlock) if(sleepBlock.Poll(Time::GetDeltaTime()))

// Allows for the code to run in play mode only
#if USE_EDITOR
#define UNO_EDITOR(returnValue) if (!Editor::IsPlayMode) { return returnValue; }
#else
#define UNO_EDITOR(returnValue)
#endif
