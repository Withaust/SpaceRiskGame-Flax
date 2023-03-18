#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/System/GameInstance.h>
#include <Game/System/GameAnalytics.h>

API_CLASS() class GAME_API Logger : public GameSystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Logger);

private:

    GameAnalytics* analytics = nullptr;

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
#define UDEBUG_STR(Text) GET_SYSTEM(Logger)->Debug(Text, __FILE__, __LINE__)
#define UDEBUG(Text, ...) GET_SYSTEM(Logger)->Debug(String::Format(Text, ##__VA_ARGS__), __FILE__, __LINE__)
#else
#define UDEBUG_STR(Text)
#define UDEBUG(Text, ...)
#endif

#define UINFO_STR(Text) GET_SYSTEM(Logger)->Info(Text, __FILE__, __LINE__)
#define UINFO(Text, ...) GET_SYSTEM(Logger)->Info(String::Format(Text, ##__VA_ARGS__), __FILE__, __LINE__)

#define UWARNING_STR(Text) GET_SYSTEM(Logger)->Warning(Text, __FILE__, __LINE__)
#define UWARNING(Text, ...) GET_SYSTEM(Logger)->Warning(String::Format(Text, ##__VA_ARGS__), __FILE__, __LINE__)

#define UERROR_STR(Text) GET_SYSTEM(Logger)->Error(Text, __FILE__, __LINE__)
#define UERROR(Text, ...) GET_SYSTEM(Logger)->Error(String::Format(Text, ##__VA_ARGS__), __FILE__, __LINE__)

#define UCRITICAL_STR(Text) GET_SYSTEM(Logger)->Critical(Text, __FILE__, __LINE__)
#define UCRITICAL(Text, ...) GET_SYSTEM(Logger)->Critical(Stringi::Format(Text, ##__VA_ARGS__), __FILE__, __LINE__)
