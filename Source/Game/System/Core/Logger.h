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

    void OnInitialize() override;
    void OnDeinitialize() override;

    API_FUNCTION() static void Print(const StringView& message);
    API_FUNCTION() static void Info(const StringView& message);

    API_FUNCTION() static void Warning(const StringView& message);
    static void Warning(const StringView& message, const char* file, int line);

    API_FUNCTION() static void Error(const StringView& message);
    static void Error(const StringView& message, const char* file, int line);
    
    API_FUNCTION() static void Critical(bool shutdown, const StringView& message);
    static void Critical(bool shutdown, const StringView& message, const char* file, int line);
};
