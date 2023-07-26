#include "Analytics.h"

UIMPL_SINGLETON(Analytics)

Analytics::Analytics(const SpawnParams& params)
    : ISystem(params)
{
}

void Analytics::Filter(StringAnsi& design)
{
    Array<char> allowedChars = { '-', '_', '.', ',', '(', ')', '!', '?', ':' };
    for (int i = 0; i < design.Length(); ++i)
    {
        char Target = design[i];
        if (Target >= 'a' && Target <= 'z')
        {
            continue;
        }
        if (Target >= 'A' && Target <= 'Z')
        {
            continue;
        }
        if (Target >= '0' && Target <= '9')
        {
            continue;
        }
        for (int j = 0; j < allowedChars.Count(); ++j)
        {
            if (Target == allowedChars[j])
            {
                continue;
            }
        }
        design[i] = ' ';
    }
    while (design.Contains("  "))
    {
        design.Replace("  ", " ");
    }
}

String Analytics::MessageTypeToString(gameanalytics::EGALoggerMessageType type)
{
    switch (type)
    {
    case gameanalytics::LogError:
    {
        return TEXT("Error");
    }
    case gameanalytics::LogWarning:
    {
        return TEXT("Warning");
    }
    case gameanalytics::LogInfo:
    {
        return TEXT("Info");
    }
    case gameanalytics::LogDebug:
    {
        return TEXT("Debug");
    }
    default:
    {
        return TEXT("");
    }
    }
}

void Analytics::OnLog(const char* message, gameanalytics::EGALoggerMessageType messageType)
{
    if (messageType == gameanalytics::EGALoggerMessageType::LogDebug || messageType == gameanalytics::EGALoggerMessageType::LogInfo)
    {
        return;
    }
    String result = String::Format(TEXT("[Analytics] {0}: {1}"), MessageTypeToString(messageType), String(message));
    LOG_STR(Info, result);
}

void Analytics::OnInitialize()
{
    if (LaunchArgs::Instance->GetArgs()->IsModded)
    {
        return;
    }

    gameanalytics::GameAnalytics::configureCustomLogHandler(std::bind(&Analytics::OnLog, this, std::placeholders::_1, std::placeholders::_2));
    gameanalytics::GameAnalytics::configureSdkGameEngineVersion(StringAnsi::Format("{}", Globals::EngineBuildNumber).GetText());
    gameanalytics::GameAnalytics::configureUserId(StringAnsi::Format("{0}", Steam::Instance->GetSteamID64()).GetText());
    gameanalytics::GameAnalytics::setEnabledErrorReporting(true);
    gameanalytics::GameAnalytics::initialize(_gameKey.GetText(), _gameSecret.GetText());
    gameanalytics::GameAnalytics::startSession();
    _initialized = true;
}

void Analytics::OnDeinitialize()
{
    if(!_initialized)
    {
        return;
    }
    gameanalytics::GameAnalytics::endSession();
    gameanalytics::GameAnalytics::onQuit();
}

void Analytics::AddResourceEvent(FlowType flowType, StringAnsi currency, float amount, StringAnsi itemType, StringAnsi itemId)
{
    if (!_initialized)
    {
        return;
    }
    gameanalytics::GameAnalytics::addResourceEvent(static_cast<gameanalytics::EGAResourceFlowType>(flowType), currency.GetText(), amount, itemType.GetText(), itemId.GetText());
}

void Analytics::AddProgressionEvent(ProgressionStatus progressionStatus, StringAnsi progression01, StringAnsi progression02, StringAnsi progression03)
{
    if (!_initialized)
    {
        return;
    }
    gameanalytics::GameAnalytics::addProgressionEvent(static_cast<gameanalytics::EGAProgressionStatus>(progressionStatus), progression01.GetText(), progression02.GetText(), progression03.GetText());
}

void Analytics::AddProgressionEvent(ProgressionStatus progressionStatus, StringAnsi progression01, StringAnsi progression02, StringAnsi progression03, int score)
{
    if (!_initialized)
    {
        return;
    }
    gameanalytics::GameAnalytics::addProgressionEvent(static_cast<gameanalytics::EGAProgressionStatus>(progressionStatus), progression01.GetText(), progression02.GetText(), progression03.GetText(), score);
}

void Analytics::AddDesignEvent(StringAnsi eventName)
{
    if (!_initialized)
    {
        return;
    }
    Filter(eventName);
    if (eventName.Contains(":"))
    {
        gameanalytics::GameAnalytics::addDesignEvent(eventName.GetText());
    }
}

void Analytics::AddDesignEvent(StringAnsi eventName, double value)
{
    if (!_initialized)
    {
        return;
    }
    Filter(eventName);
    if (eventName.Contains(":"))
    {
        gameanalytics::GameAnalytics::addDesignEvent(eventName.GetText(), value);
    }
}

void Analytics::AddErrorEvent(ErrorSeverity severity, const char* message)
{
    if (!_initialized)
    {
        return;
    }
    gameanalytics::GameAnalytics::addErrorEvent(static_cast<gameanalytics::EGAErrorSeverity>(severity), message);
}

void Analytics::AddErrorEvent(ErrorSeverity severity, const StringAnsi& message)
{
    if (!_initialized)
    {
        return;
    }
    AddErrorEvent(severity, message.GetText());
}

void Analytics::AddErrorEvent(ErrorSeverity severity, const String& message)
{
    if (!_initialized)
    {
        return;
    }
    AddErrorEvent(severity, message.ToStringAnsi());
}
