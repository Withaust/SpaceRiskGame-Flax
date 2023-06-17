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

public:
    API_FIELD() static Logger* Instance;

    void OnInitialize() override;
    void OnDeinitialize() override;

    API_FUNCTION(Attributes = "Cmd") void Print(const StringView& message);
    API_FUNCTION(Attributes = "Cmd") void Info(const StringView& message);

    API_FUNCTION(Attributes = "Cmd") void Warning(const StringView& message);
    void Warning(const StringView& message, const char* file, int line);

    API_FUNCTION(Attributes = "Cmd") void Error(const StringView& message);
    void Error(const StringView& message, const char* file, int line);
    
    API_FUNCTION(Attributes = "Cmd") void Critical(bool shutdown, const StringView& message);
    void Critical(bool shutdown, const StringView& message, const char* file, int line);
};
