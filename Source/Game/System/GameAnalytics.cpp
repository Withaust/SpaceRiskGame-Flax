#include "GameAnalytics.h"

GameAnalytics::GameAnalytics(const SpawnParams& params)
    : GameSystem(params)
{

}

String GameAnalytics::MessageTypeToString(gameanalytics::EGALoggerMessageType Type)
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

void GameAnalytics::OnLog(const char* Message, gameanalytics::EGALoggerMessageType MessageType)
{
    String Result = String::Format(TEXT("[GA] {0}: {1}"), MessageTypeToString(MessageType), String(Message));
    LOG_STR(Info, Result);
}

void GameAnalytics::OnInitialize()
{
    gameanalytics::StringVector Dimensions;
    Dimensions.add("Debug");
    Dimensions.add("Modded");
    Dimensions.add("Vanilla");
    gameanalytics::GameAnalytics::configureAvailableCustomDimensions01(Dimensions);

    gameanalytics::GameAnalytics::configureCustomLogHandler(std::bind(&GameAnalytics::OnLog, this, std::placeholders::_1, std::placeholders::_2));

    StringAnsi EngineVersion = StringAnsi::Format("Flax {0}", Globals::EngineBuildNumber);
    gameanalytics::GameAnalytics::configureSdkGameEngineVersion(EngineVersion.GetText());

    StringAnsi ID = StringAnsi::Format("{0}", GameInstance::GetInstance()->Get<Steam>()->GetSteamID64());
    gameanalytics::GameAnalytics::configureUserId(ID.GetText());

    gameanalytics::GameAnalytics::setEnabledInfoLog(true);
#ifdef BUILD_DEBUG
    gameanalytics::GameAnalytics::setEnabledVerboseLog(true);
#endif
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

void GameAnalytics::OnDeinitialize()
{
    gameanalytics::GameAnalytics::endSession();
    gameanalytics::GameAnalytics::onQuit();
}

void GameAnalytics::AddResourceEvent(FlowType flowType, const char* currency, float amount, const char* itemType, const char* itemId)
{
    gameanalytics::GameAnalytics::addResourceEvent(static_cast<gameanalytics::EGAResourceFlowType>(flowType), currency, amount, itemType, itemId);
}

void GameAnalytics::AddProgressionEvent(ProgressionStatus progressionStatus, const char* progression01, const char* progression02, const char* progression03)
{
    gameanalytics::GameAnalytics::addProgressionEvent(static_cast<gameanalytics::EGAProgressionStatus>(progressionStatus), progression01, progression02, progression03);
}

void GameAnalytics::AddProgressionEvent(ProgressionStatus progressionStatus, const char* progression01, const char* progression02, const char* progression03, int score)
{
    gameanalytics::GameAnalytics::addProgressionEvent(static_cast<gameanalytics::EGAProgressionStatus>(progressionStatus), progression01, progression02, progression03, score);
}

void GameAnalytics::AddDesignEvent(const char* eventId)
{
    gameanalytics::GameAnalytics::addDesignEvent(eventId);
}

void GameAnalytics::AddDesignEvent(const char* eventId, double value)
{
    gameanalytics::GameAnalytics::addDesignEvent(eventId, value);
}

void GameAnalytics::AddErrorEvent(ErrorSeverity severity, const char* message)
{
    gameanalytics::GameAnalytics::addErrorEvent(static_cast<gameanalytics::EGAErrorSeverity>(severity), message);
}
