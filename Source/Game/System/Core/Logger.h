#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Platform/Platform.h>
#include <Engine/Debug/DebugLog.h>

#include <Game/System/Core.h>
#include <Game/System/Core/Analytics.h>

API_CLASS() class GAME_API Logger : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Logger);

    void OnMessageCapture(LogType type, const StringView& message);

public:

    API_FIELD() static Logger* Instance;

    void OnInitialize() override;
    void OnDeinitialize() override;

    API_EVENT() Delegate<const StringView&> OnInfo;
    API_EVENT() Delegate<const StringView&> OnWarning;
    API_EVENT() Delegate<const StringView&> OnError;
    API_EVENT() Delegate<const StringView&> OnFatal;

    API_FUNCTION() void Info(const StringView& message);

    API_FUNCTION() void Warning(const StringView& message);
    void Warning(const StringView& message, const char* file, int line);

    API_FUNCTION() void Error(const StringView& message);
    void Error(const StringView& message, const char* file, int line);
    
    API_FUNCTION() void Critical(bool shutdown, const StringView& message);
    void Critical(bool shutdown, const StringView& message, const char* file, int line);
};
