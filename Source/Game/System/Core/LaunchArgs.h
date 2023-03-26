#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Engine/Engine.h>
#include <Engine/Platform/Platform.h>
#include <Engine/Platform/FileSystem.h>
#include <Engine/Platform/File.h>
#include <Engine/Debug/DebugLog.h>
#include <Engine/Serialization/JsonSerializer.h>
#include <Engine/Engine/Globals.h>
#include <Engine/Core/Formatting.h>

#include <Game/System/Core/CoreInstance.h>

API_CLASS() class GAME_API Args : public ISerializable
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(Args);
public:
    API_FIELD()
        bool IsSteam = false;
    API_FIELD()
        bool IsHost = true;
    API_FIELD()
        String Hostname;
    API_FIELD()
        int Port = 40808;
    API_FIELD()
        bool IsModded = false;
    API_FIELD()
        String Core;
};

API_CLASS() class GAME_API LaunchArgs : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(LaunchArgs);
    static LaunchArgs* Get() { return CoreInstance::Instance()->Get<LaunchArgs>(); }

private:

    Args* _args = nullptr;
    String GetNextArgument(int currentIndex, const Array<String>& args);

public:
    void OnInitialize() override;
    void OnDeinitialize() override;
    const Args* GetArgs();
};
