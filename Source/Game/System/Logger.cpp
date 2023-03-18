#include "Logger.h"

#include <Engine/Platform/Platform.h>
#include <Engine/Debug/DebugLog.h>

Logger::Logger(const SpawnParams& params)
    : GameSystem(params)
{

}

void Logger::OnInitialize()
{
    analytics = GET_SYSTEM(GameAnalytics);
}

void Logger::OnDeinitialize()
{

}

void Logger::Debug(const StringView& Message, const char* File, int Line)
{
    LOG_STR(Info, Message);
    analytics->AddErrorEvent(GameAnalytics::ErrorSeverity::Debug, String::Format(TEXT("{0} in file {1} at line {2}"), Message, String(File), Line).ToStringAnsi().GetText());
}

void Logger::Info(const StringView& Message, const char* File, int Line)
{
#ifdef BUILD_DEBUG
    DebugLog::Log(Message);
#endif
    analytics->AddErrorEvent(GameAnalytics::ErrorSeverity::Info, String::Format(TEXT("{0} in file {1} at line {2}"), Message, String(File), Line).ToStringAnsi().GetText());
}

void Logger::Warning(const StringView& Message, const char* File, int Line)
{
#ifdef BUILD_DEBUG
    DebugLog::LogWarning(Message);
#endif
    analytics->AddErrorEvent(GameAnalytics::ErrorSeverity::Warning, String::Format(TEXT("{0} in file {1} at line {2}"), Message, String(File), Line).ToStringAnsi().GetText());
}

void Logger::Error(const StringView& Message, const char* File, int Line)
{
#ifdef BUILD_DEBUG
    DebugLog::LogError(Message);
#endif
    analytics->AddErrorEvent(GameAnalytics::ErrorSeverity::Error, String::Format(TEXT("{0} in file {1} at line {2}"), Message, String(File), Line).ToStringAnsi().GetText());
}

void Logger::Critical(const StringView& Message, const char* File, int Line)
{
#ifdef BUILD_DEBUG
    DebugLog::LogError(Message);
#endif
    analytics->AddErrorEvent(GameAnalytics::ErrorSeverity::Critical, String::Format(TEXT("{0} in file {1} at line {2}"), Message, String(File), Line).ToStringAnsi().GetText());
    Platform::Error(Message);
    Engine::RequestExit(1);
}
