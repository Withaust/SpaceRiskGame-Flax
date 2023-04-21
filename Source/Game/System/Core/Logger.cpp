#include "Logger.h"

Logger::Logger(const SpawnParams& params)
    : ISystem(params)
{
}

void Logger::OnInitialize()
{
}

void Logger::OnDeinitialize()
{
}

void Logger::Print(const StringView& message, const char* file, int line)
{
#ifdef BUILD_DEBUG
    LOG_STR(Info, message);
#endif
}

void Logger::Info(const StringView& message, const char* file, int line)
{
#ifdef BUILD_DEBUG
    DebugLog::Log(message);
#endif
}

void Logger::Warning(const StringView& message, const char* file, int line)
{
#ifdef BUILD_DEBUG
    DebugLog::LogWarning(message);
#endif
    Analytics::Get()->AddErrorEvent(Analytics::ErrorSeverity::Warning, String::Format(TEXT("{0} in file {1} at line {2}"), message, String(file), line).ToStringAnsi().GetText());
}

void Logger::Error(const StringView& message, const char* file, int line)
{
#ifdef BUILD_DEBUG
    DebugLog::LogError(message);
#endif
    Analytics::Get()->AddErrorEvent(Analytics::ErrorSeverity::Error, String::Format(TEXT("{0} in file {1} at line {2}"), message, String(file), line).ToStringAnsi().GetText());
}

void Logger::Critical(bool shutdown, const StringView& message, const char* file, int line)
{
#ifdef BUILD_DEBUG
    DebugLog::LogError(message);
#endif
    Analytics::Get()->AddErrorEvent(Analytics::ErrorSeverity::Critical, String::Format(TEXT("{0} in file {1} at line {2}"), message, String(file), line).ToStringAnsi().GetText());
    if (shutdown)
    {
        Platform::Error(message);
        Engine::RequestExit(1);
    }
}
