#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Platform/Platform.h>
#include <Engine/Debug/DebugLog.h>

#include <Game/System/Core/CoreInstance.h>
#include <Game/System/Core/Analytics.h>

API_CLASS() class GAME_API Logger : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Logger);
    USYSTEM(Logger);

private:

    Analytics* analytics = nullptr;

public:

    void OnInitialize() override;
    void OnDeinitialize() override;

    void Debug(const StringView& Message, const char* File, int Line);
    void Info(const StringView& Message, const char* File, int Line);
    void Warning(const StringView& Message, const char* File, int Line);
    void Error(const StringView& Message, const char* File, int Line);
    void Critical(const StringView& Message, const char* File, int Line);
};

#ifdef BUILD_DEBUG
#define UDEBUG_STR(Text) Logger::Get()->Debug(Text, __FILE__, __LINE__)
#define UDEBUG(Text, ...) Logger::Get()->Debug(String::Format(Text, ##__VA_ARGS__), __FILE__, __LINE__)
#else
#define UDEBUG_STR(Text)
#define UDEBUG(Text, ...)
#endif

#define UINFO_STR(Text) Logger::Get()->Info(Text, __FILE__, __LINE__)
#define UINFO(Text, ...) Logger::Get()->Info(String::Format(Text, ##__VA_ARGS__), __FILE__, __LINE__)

#define UWARNING_STR(Text) Logger::Get()->Warning(Text, __FILE__, __LINE__)
#define UWARNING(Text, ...) Logger::Get()->Warning(String::Format(Text, ##__VA_ARGS__), __FILE__, __LINE__)

#define UERROR_STR(Text) Logger::Get()->Error(Text, __FILE__, __LINE__)
#define UERROR(Text, ...) Logger::Get()->Error(String::Format(Text, ##__VA_ARGS__), __FILE__, __LINE__)

#define UCRITICAL_STR(Text) Logger::Get()->Critical(Text, __FILE__, __LINE__)
#define UCRITICAL(Text, ...) Logger::Get()->Critical(Stringi::Format(Text, ##__VA_ARGS__), __FILE__, __LINE__)
