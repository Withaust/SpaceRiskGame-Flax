#pragma once

#pragma warning( disable : 4099 )

#include <Engine/Scripting/Script.h>
#include <Engine/Debug/DebugLog.h>
#include <Engine/Engine/Engine.h>
#include <Engine/Engine/Globals.h>

#include <Game/System/CoreInstance.h>
#include <Game/System/Core/LaunchArgs.h>
#include <Game/System/Core/Steam.h>

#include <GameAnalytics/GameAnalytics.h>

class DesignEvent
{
private:
    StringAnsi _result;
    void Filter(String& design);
    void FilterAnsi(StringAnsi& design);

public:

    const char* GetResult()
    {
        return _result.GetText();
    }

    DesignEvent(StringAnsi part1);
    DesignEvent(StringAnsi part1, StringAnsi part2);
    DesignEvent(StringAnsi part1, StringAnsi part2, StringAnsi part3);
    DesignEvent(StringAnsi part1, StringAnsi part2, StringAnsi part3, StringAnsi part4);
    DesignEvent(StringAnsi part1, StringAnsi part2, StringAnsi part3, StringAnsi part4, StringAnsi part5);
};

API_CLASS() class GAME_API Analytics : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Analytics);
    static Analytics* Get() { return CoreInstance::Instance()->Get<Analytics>(); }
private:

    bool _initialized = false;
    StringAnsi _gameKey = "d5d99280fc61340c92a04b7520ddcdda";
    StringAnsi _gameSecret = "215fe92fa38665c64ebb197279e549c6b0c88ec3";

public:

    String MessageTypeToString(gameanalytics::EGALoggerMessageType type);
    void OnLog(const char* message, gameanalytics::EGALoggerMessageType messageType);

    void OnInitialize() override;
    void OnDeinitialize() override;

    /// <summary>
    /// This enum is used to specify flow in resource events
    /// </summary>
    enum class FlowType
    {
        /// <summary>
        /// Used when adding to a resource currency
        /// </summary>
        Source = 1,
        /// <summary>
        /// Used when subtracting from a resource currency
        /// </summary>
        Sink = 2
    };

    /// <summary>
    /// This enum is used to specify status for progression event
    /// </summary>
    enum class ProgressionStatus
    {
        /// <summary>
        /// User started progression
        /// </summary>
        Start = 1,
        /// <summary>
        /// User succesfully ended a progression
        /// </summary>
        Complete = 2,
        /// <summary>
        /// User failed a progression
        /// </summary>
        Fail = 3
    };

    /// <summary>
    /// This enum is used to specify severity of an error event
    /// </summary>
    enum class ErrorSeverity
    {
        Debug = 1,
        Info = 2,
        Warning = 3,
        Error = 4,
        Critical = 5
    };

public:

    void AddResourceEvent(FlowType flowType, const char* currency, float amount, const char* itemType, const char* itemId);

    void AddProgressionEvent(ProgressionStatus progressionStatus, const char* progression01, const char* progression02, const char* progression03);
    void AddProgressionEvent(ProgressionStatus progressionStatus, const char* progression01, const char* progression02, const char* progression03, int score);

    void AddDesignEvent(DesignEvent event);
    void AddDesignEvent(DesignEvent event, double value);

    void AddErrorEvent(ErrorSeverity severity, const char* message);
    void AddErrorEvent(ErrorSeverity severity, const StringAnsi& message);
    void AddErrorEvent(ErrorSeverity severity, const String& message);
};
