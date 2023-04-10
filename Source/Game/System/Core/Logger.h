#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Platform/Platform.h>
#include <Engine/Debug/DebugLog.h>

#include <Game/System/CoreInstance.h>
#include <Game/System/Core/Analytics.h>

API_CLASS() class GAME_API Logger : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Logger);
    static Logger* Get() { return CoreInstance::Instance()->Get<Logger>(); }

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
