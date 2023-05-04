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

    void Print(const StringView& message, const char* file, int line);
    void Info(const StringView& message, const char* file, int line);
    void Warning(const StringView& message, const char* file, int line);
    void Error(const StringView& message, const char* file, int line);
    void Critical(bool shutdown, const StringView& message, const char* file, int line);
};
