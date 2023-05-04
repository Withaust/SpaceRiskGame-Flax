#include "Analytics.h"

void DesignEvent::Filter(String& design)
{
    Array<Char> allowedChars = { TEXT('-'), TEXT('_'), TEXT('.'), TEXT(','), TEXT('('), TEXT(')'), TEXT('!'), TEXT('?') };
    for (int i = 0; i < design.Length(); ++i)
    {
        Char Target = design[i];
        if (Target >= TEXT('a') && Target <= TEXT('z'))
        {
            continue;
        }
        if (Target >= TEXT('A') && Target <= TEXT('Z'))
        {
            continue;
        }
        if (Target >= TEXT('0') && Target <= TEXT('9'))
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
        design[i] = TEXT(' ');
    }
    while (design.Contains(TEXT("  ")))
    {
        design.Replace(TEXT("  "), TEXT(" "));
    }
}

void DesignEvent::FilterAnsi(StringAnsi& design)
{
    Array<char> allowedChars = { '-', '_', '.', ',', '(', ')', '!', '?' };
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

DesignEvent::DesignEvent(StringAnsi part1)
{
    FilterAnsi(part1);
    _result = part1;
}

DesignEvent::DesignEvent(StringAnsi part1, StringAnsi part2)
{
    FilterAnsi(part1);
    FilterAnsi(part2);
    _result = StringAnsi::Format("{0}:{1}", part1, part2);
}

DesignEvent::DesignEvent(StringAnsi part1, StringAnsi part2, StringAnsi part3)
{
    FilterAnsi(part1);
    FilterAnsi(part2);
    FilterAnsi(part3);
    _result = StringAnsi::Format("{0}:{1}:{2}", part1, part2, part3);
}


DesignEvent::DesignEvent(StringAnsi part1, StringAnsi part2, StringAnsi part3, StringAnsi part4)
{
    FilterAnsi(part1);
    FilterAnsi(part2);
    FilterAnsi(part3);
    FilterAnsi(part4);
    _result = StringAnsi::Format("{0}:{1}:{2}:{3}", part1, part2, part3, part4);
}


DesignEvent::DesignEvent(StringAnsi part1, StringAnsi part2, StringAnsi part3, StringAnsi part4, StringAnsi part5)
{
    FilterAnsi(part1);
    FilterAnsi(part2);
    FilterAnsi(part3);
    FilterAnsi(part4);
    FilterAnsi(part5);
    _result = StringAnsi::Format("{0}:{1}:{2}:{3}:{4}", part1, part2, part3, part4, part5);
}

Analytics::Analytics(const SpawnParams& params)
    : ISystem(params)
{
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
    String result = String::Format(TEXT("[GA] {0}: {1}"), MessageTypeToString(messageType), String(message));
    LOG_STR(Info, result);
}

void Analytics::OnInitialize()
{
    gameanalytics::StringVector dimensions;
    dimensions.add("Debug");
    dimensions.add("Modded");
    dimensions.add("Vanilla");
    gameanalytics::GameAnalytics::configureAvailableCustomDimensions01(dimensions);

    gameanalytics::GameAnalytics::configureCustomLogHandler(std::bind(&Analytics::OnLog, this, std::placeholders::_1, std::placeholders::_2));

    StringAnsi engineVersion = StringAnsi::Format("Flax {0}", Globals::EngineBuildNumber);
    gameanalytics::GameAnalytics::configureSdkGameEngineVersion(engineVersion.GetText());

    StringAnsi id = StringAnsi::Format("{0}", Core::Get<Steam>()->GetSteamID64());
    gameanalytics::GameAnalytics::configureUserId(id.GetText());

    gameanalytics::GameAnalytics::setEnabledErrorReporting(true);

    const Args* args = Core::Get<LaunchArgs>()->GetArgs();

#ifdef BUILD_DEBUG
    gameanalytics::GameAnalytics::setCustomDimension01("Debug");
#else
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
}

void Analytics::OnDeinitialize()
{
    gameanalytics::GameAnalytics::endSession();
    gameanalytics::GameAnalytics::onQuit();
}

void Analytics::AddResourceEvent(FlowType flowType, const char* currency, float amount, const char* itemType, const char* itemId)
{
    gameanalytics::GameAnalytics::addResourceEvent(static_cast<gameanalytics::EGAResourceFlowType>(flowType), currency, amount, itemType, itemId);
}

void Analytics::AddProgressionEvent(ProgressionStatus progressionStatus, const char* progression01, const char* progression02, const char* progression03)
{
    gameanalytics::GameAnalytics::addProgressionEvent(static_cast<gameanalytics::EGAProgressionStatus>(progressionStatus), progression01, progression02, progression03);
}

void Analytics::AddProgressionEvent(ProgressionStatus progressionStatus, const char* progression01, const char* progression02, const char* progression03, int score)
{
    gameanalytics::GameAnalytics::addProgressionEvent(static_cast<gameanalytics::EGAProgressionStatus>(progressionStatus), progression01, progression02, progression03, score);
}

void Analytics::AddDesignEvent(DesignEvent event)
{
    gameanalytics::GameAnalytics::addDesignEvent(event.GetResult());
}

void Analytics::AddDesignEvent(DesignEvent event, double value)
{
    gameanalytics::GameAnalytics::addDesignEvent(event.GetResult(), value);
}

void Analytics::AddErrorEvent(ErrorSeverity severity, const char* message)
{
    gameanalytics::GameAnalytics::addErrorEvent(static_cast<gameanalytics::EGAErrorSeverity>(severity), message);
}

void Analytics::AddErrorEvent(ErrorSeverity severity, const StringAnsi& message)
{
    AddErrorEvent(severity, message.GetText());
}

void Analytics::AddErrorEvent(ErrorSeverity severity, const String& message)
{
    AddErrorEvent(severity, message.ToStringAnsi());
}
