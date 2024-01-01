#include "Logger.h"

Logger* Logger::Instance = nullptr;

Logger::Logger(const SpawnParams& params)
    : ISystem(params)
{
}

void Logger::OnMessageCapture(LogType type, const StringView& message)
{
    switch (type)
    {
    case LogType::Info:
        OnInfo(message);
        break;
    case LogType::Warning:
        OnWarning(message);
        break;
    case LogType::Error:
        OnError(message);
        break;
    case LogType::Fatal:
        OnFatal(message);
        break;
    default:
        OnInfo(message);
        break;
    }
}

void Logger::OnInitialize()
{
    Log::Logger::OnMessage.Bind<Logger, &Logger::OnMessageCapture>(this);
    Log::Logger::OnError.Bind<Logger, &Logger::OnMessageCapture>(this);
}

void Logger::OnDeinitialize()
{
    Log::Logger::OnMessage.Unbind<Logger, &Logger::OnMessageCapture>(this);
    Log::Logger::OnError.Unbind<Logger, &Logger::OnMessageCapture>(this);
}

void Logger::Info(const StringView& message)
{
    DebugLog::Log(message);
}

void Logger::Warning(const StringView& message)
{
    DebugLog::LogWarning(message);
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
    DebugLog::LogError(message);
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
    DebugLog::LogError(message);
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
