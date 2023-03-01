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

#include <Game/System/GameSystem.h>

API_CLASS() class GAME_API LaunchFile : public ISerializable
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(LaunchFile);
public:
    API_FIELD()
        bool IsSteam = false;
    API_FIELD()
        bool IsHost = true;
    API_FIELD()
        String Hostname;
    API_FIELD()
        int Port = 40808;
};

API_CLASS() class GAME_API LaunchArgs : public GameSystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(LaunchArgs);
private:

    LaunchFile* _file = nullptr;
    String GetNextArgument(int currentIndex, const Array<String>& args);

public:
    void OnInitialize() override;
    void OnDeinitialize() override;
    const LaunchFile* GetFile();
};
