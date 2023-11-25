#include "Logger.h"

Logger* Logger::Instance = nullptr;

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

void Logger::Print(const StringView& message)
{
#ifdef BUILD_DEBUG
    LOG_STR(Info, message);
#endif
}

void Logger::Info(const StringView& message)
{
#ifdef BUILD_DEBUG
    DebugLog::Log(message);
#endif
}

void Logger::Warning(const StringView& message)
{
#ifdef BUILD_DEBUG
    DebugLog::LogWarning(message);
#endif
    if (Analytics::Instance)
    {
        Analytics::Instance->AddErrorEvent(Analytics::ErrorSeverity::Warning, message);
    }
}

void Logger::Warning(const StringView& message, const char* file, int line)
{
    Warning(String::Format(TEXT("{0} in file {1} at line {2}"), message, String(file), line));
}

void Logger::Error(const StringView& message)
{
#ifdef BUILD_DEBUG
    DebugLog::LogError(message);
#endif
    if (Analytics::Instance)
    {
        Analytics::Instance->AddErrorEvent(Analytics::ErrorSeverity::Error, message);
    }
}

void Logger::Error(const StringView& message, const char* file, int line)
{
    Error(String::Format(TEXT("{0} in file {1} at line {2}"), message, String(file), line));
}

void Logger::Critical(bool shutdown, const StringView& message)
{
#ifdef BUILD_DEBUG
    DebugLog::LogError(message);
#endif
    if (Analytics::Instance)
    {
        Analytics::Instance->AddErrorEvent(Analytics::ErrorSeverity::Critical, message);
    }
#ifdef BUILD_DEBUG
    Platform::Error(message);
    if (shutdown)
    {
        Engine::RequestExit(1);
    }
#else
    if (shutdown)
    {
        Platform::Error(message);
        Engine::RequestExit(1);
    }
#endif
}

void Logger::Critical(bool shutdown, const StringView& message, const char* file, int line)
{
    Critical(shutdown, String::Format(TEXT("{0} in file {1} at line {2}"), message, String(file), line));
}
