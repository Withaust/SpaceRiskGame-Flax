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
#if BUILD_DEBUG == 0
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
#endif // BUILD_DEBUG
void Analytics::OnInitialize()
{
#if BUILD_DEBUG == 0
    gameanalytics::StringVector dimensions;
    dimensions.add("Debug");
    dimensions.add("Modded");
    dimensions.add("Vanilla");
    gameanalytics::GameAnalytics::configureAvailableCustomDimensions01(dimensions);

    gameanalytics::GameAnalytics::configureCustomLogHandler(std::bind(&Analytics::OnLog, this, std::placeholders::_1, std::placeholders::_2));

    StringAnsi engineVersion = StringAnsi::Format("Flax {0}", Globals::EngineBuildNumber);
    gameanalytics::GameAnalytics::configureSdkGameEngineVersion(engineVersion.GetText());

    StringAnsi id = StringAnsi::Format("{0}", Steam::Instance->GetSteamID64());
    gameanalytics::GameAnalytics::configureUserId(id.GetText());

    gameanalytics::GameAnalytics::setEnabledErrorReporting(true);

    const Args* args = LaunchArgs::Instance->GetArgs();
#if BUILD_DEVELOPMENT == 1
    gameanalytics::GameAnalytics::setCustomDimension01("Debug");
#elif BUILD_RELEASE == 1
    if (args->IsModded)
    {
        gameanalytics::GameAnalytics::setCustomDimension01("Modded");
    }
    else
    {
        gameanalytics::GameAnalytics::setCustomDimension01("Vanilla");
    }
#endif
    gameanalytics::StringVector coreMods;
    coreMods.add(args->Core.ToStringAnsi().GetText());
    gameanalytics::GameAnalytics::configureAvailableCustomDimensions02(coreMods);
    gameanalytics::GameAnalytics::setCustomDimension02(args->Core.ToStringAnsi().GetText());

    gameanalytics::GameAnalytics::initialize(_gameKey.GetText(), _gameSecret.GetText());
    gameanalytics::GameAnalytics::startSession();

    _initialized = true;
#endif // BUILD_DEBUG
}

void Analytics::OnDeinitialize()
{
#if BUILD_DEBUG == 0
    gameanalytics::GameAnalytics::endSession();
    gameanalytics::GameAnalytics::onQuit();
#endif // BUILD_DEBUG
}

void Analytics::AddResourceEvent(FlowType flowType, StringAnsi currency, float amount, StringAnsi itemType, StringAnsi itemId)
{
#if BUILD_DEBUG == 0
    gameanalytics::GameAnalytics::addResourceEvent(static_cast<gameanalytics::EGAResourceFlowType>(flowType), currency.GetText(), amount, itemType.GetText(), itemId.GetText());
#endif // BUILD_DEBUG
}

void Analytics::AddProgressionEvent(ProgressionStatus progressionStatus, StringAnsi progression01, StringAnsi progression02, StringAnsi progression03)
{
#if BUILD_DEBUG == 0
    gameanalytics::GameAnalytics::addProgressionEvent(static_cast<gameanalytics::EGAProgressionStatus>(progressionStatus), progression01.GetText(), progression02.GetText(), progression03.GetText());
#endif // BUILD_DEBUG
}

void Analytics::AddProgressionEvent(ProgressionStatus progressionStatus, StringAnsi progression01, StringAnsi progression02, StringAnsi progression03, int score)
{
#if BUILD_DEBUG == 0
    gameanalytics::GameAnalytics::addProgressionEvent(static_cast<gameanalytics::EGAProgressionStatus>(progressionStatus), progression01.GetText(), progression02.GetText(), progression03.GetText(), score);
#endif // BUILD_DEBUG
}

void Analytics::AddDesignEvent(StringAnsi eventName)
{
#if BUILD_DEBUG == 0
    Filter(eventName);
    if (eventName.Contains(":"))
    {
        gameanalytics::GameAnalytics::addDesignEvent(eventName.GetText());
    }
#endif // BUILD_DEBUG
}

void Analytics::AddDesignEvent(StringAnsi eventName, double value)
{
#if BUILD_DEBUG == 0
    Filter(eventName);
    if (eventName.Contains(":"))
    {
        gameanalytics::GameAnalytics::addDesignEvent(eventName.GetText(), value);
    }
#endif // BUILD_DEBUG
}

void Analytics::AddErrorEvent(ErrorSeverity severity, const char* message)
{
#if BUILD_DEBUG == 0
    gameanalytics::GameAnalytics::addErrorEvent(static_cast<gameanalytics::EGAErrorSeverity>(severity), message);
#endif // BUILD_DEBUG
}

void Analytics::AddErrorEvent(ErrorSeverity severity, const StringAnsi& message)
{
#if BUILD_DEBUG == 0
    AddErrorEvent(severity, message.GetText());
#endif // BUILD_DEBUG
}

void Analytics::AddErrorEvent(ErrorSeverity severity, const String& message)
{
#if BUILD_DEBUG == 0
    AddErrorEvent(severity, message.ToStringAnsi());
#endif // BUILD_DEBUG
}
