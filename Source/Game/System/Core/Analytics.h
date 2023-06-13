#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Debug/DebugLog.h>
#include <Engine/Engine/Engine.h>
#include <Engine/Engine/Globals.h>

#include <Game/System/Core.h>
#include <Game/System/Core/LaunchArgs.h>
#include <Game/System/Core/Steam.h>

#if BUILD_DEBUG == 0
__pragma(warning(push))
#pragma warning(push)
__pragma(warning(disable : 4099))
#pragma warning( disable : 4099 )
#include <GameAnalytics/GameAnalytics.h>
__pragma (warning(pop))
#pragma warning(pop)
#endif // BUILD_DEBUG

API_CLASS() class GAME_API Analytics : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Analytics);
private:

    bool _initialized = false;
    StringAnsi _gameKey = "d5d99280fc61340c92a04b7520ddcdda";
    StringAnsi _gameSecret = "215fe92fa38665c64ebb197279e549c6b0c88ec3";
    void Filter(StringAnsi& design);

public:
    API_FIELD() static Analytics* Instance;
#if BUILD_DEBUG == 0
    String MessageTypeToString(gameanalytics::EGALoggerMessageType type);
    void OnLog(const char* message, gameanalytics::EGALoggerMessageType messageType);
#endif // BUILD_DEBUG
    void OnInitialize() override;
    void OnDeinitialize() override;

    /// <summary>
    /// This enum is used to specify flow in resource events
    /// </summary>
    API_ENUM() enum class FlowType
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
    API_ENUM() enum class ProgressionStatus
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
    API_ENUM() enum class ErrorSeverity
    {
        Debug = 1,
        Info = 2,
        Warning = 3,
        Error = 4,
        Critical = 5
    };

public:

    API_FUNCTION() void AddResourceEvent(FlowType flowType, StringAnsi currency, float amount, StringAnsi itemType, StringAnsi itemId);

    API_FUNCTION() void AddProgressionEvent(ProgressionStatus progressionStatus, StringAnsi progression01, StringAnsi progression02, StringAnsi progression03);
    API_FUNCTION() void AddProgressionEvent(ProgressionStatus progressionStatus, StringAnsi progression01, StringAnsi progression02, StringAnsi progression03, int score);

    API_FUNCTION() void AddDesignEvent(StringAnsi eventName);
    API_FUNCTION() void AddDesignEvent(StringAnsi eventName, double value);

    void AddErrorEvent(ErrorSeverity severity, const char* message);
    void AddErrorEvent(ErrorSeverity severity, const StringAnsi& message);
    API_FUNCTION() void AddErrorEvent(ErrorSeverity severity, const String& message);
};
