#include "Analytics.h"

Analytics::Analytics(const SpawnParams& params)
    : ISystem(params)
{

}

String Analytics::MessageTypeToString(gameanalytics::EGALoggerMessageType Type)
{
    switch (Type)
    {
    case gameanalytics::LogError:
        return TEXT("Error");
        break;
    case gameanalytics::LogWarning:
        return TEXT("Warning");
        break;
    case gameanalytics::LogInfo:
        return TEXT("Info");
        break;
    case gameanalytics::LogDebug:
        return TEXT("Debug");
        break;
    default:
        return TEXT("");
        break;
    }
}

void Analytics::OnLog(const char* Message, gameanalytics::EGALoggerMessageType MessageType)
{
    String Result = String::Format(TEXT("[GA] {0}: {1}"), MessageTypeToString(MessageType), String(Message));
    LOG_STR(Info, Result);
}

void Analytics::FilterDesign(String& Design)
{
    Array<Char> AllowedChars = { TEXT('-'), TEXT('_'), TEXT('.'), TEXT(','), TEXT('('), TEXT(')'), TEXT('!'), TEXT('?') };
    for (int i = 0; i < Design.Length(); ++i)
    {
        Char Target = Design[i];
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
        for (int j = 0; j < AllowedChars.Count(); ++j)
        {
            if (Target == AllowedChars[j])
            {
                continue;
            }
        }
        Design[i] = TEXT(' ');
    }
    while (Design.Contains(TEXT("  ")))
    {
        Design.Replace(TEXT("  "), TEXT(" "));
    }
}

void Analytics::FilterDesignAnsi(StringAnsi& Design)
{
    Array<char> AllowedChars = { '-', '_', '.', ',', '(', ')', '!', '?' };
    for (int i = 0; i < Design.Length(); ++i)
    {
        char Target = Design[i];
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
        for (int j = 0; j < AllowedChars.Count(); ++j)
        {
            if (Target == AllowedChars[j])
            {
                continue;
            }
        }
        Design[i] = ' ';
    }
    while (Design.Contains("  "))
    {
        Design.Replace("  ", " ");
    }
}

void Analytics::OnInitialize()
{
    gameanalytics::StringVector Dimensions;
    Dimensions.add("Debug");
    Dimensions.add("Modded");
    Dimensions.add("Vanilla");
    gameanalytics::GameAnalytics::configureAvailableCustomDimensions01(Dimensions);

    gameanalytics::GameAnalytics::configureCustomLogHandler(std::bind(&Analytics::OnLog, this, std::placeholders::_1, std::placeholders::_2));

    StringAnsi EngineVersion = StringAnsi::Format("Flax {0}", Globals::EngineBuildNumber);
    gameanalytics::GameAnalytics::configureSdkGameEngineVersion(EngineVersion.GetText());

    StringAnsi ID = StringAnsi::Format("{0}", CoreInstance::GetInstance()->Get<Steam>()->GetSteamID64());
    gameanalytics::GameAnalytics::configureUserId(ID.GetText());

    gameanalytics::GameAnalytics::setEnabledInfoLog(true);
    gameanalytics::GameAnalytics::setEnabledErrorReporting(true);

    const Args* args = GET_SYSTEM(LaunchArgs)->GetArgs();

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

    gameanalytics::StringVector CoreMods;
    CoreMods.add(args->Core.ToStringAnsi().GetText());
    gameanalytics::GameAnalytics::configureAvailableCustomDimensions02(CoreMods);
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

void Analytics::AddDesignEvent(const char* eventId)
{
    gameanalytics::GameAnalytics::addDesignEvent(eventId);
}

void Analytics::AddDesignEvent(const StringAnsi& eventId)
{
    AddDesignEvent(eventId.GetText());
}

void Analytics::AddDesignEvent(const char* eventId, double value)
{
    gameanalytics::GameAnalytics::addDesignEvent(eventId, value);
}

void Analytics::AddDesignEvent(const StringAnsi& eventId, double value)
{
    AddDesignEvent(eventId.GetText(), value);
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
