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

#include <Game/System/Core.h>

API_CLASS() class GAME_API LaunchArgs : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(LaunchArgs);

private:

    String GetNextArgument(int currentIndex, const Array<String>& args);

public:
    API_FIELD() static LaunchArgs* Instance;

    // Steam
    API_FIELD() static bool IsSteam;
    API_FIELD() static String Name;
    // Networking
    API_FIELD() static bool IsHost;
    API_FIELD() static String Hostname;
    // Mod support
    API_FIELD() static bool IsModded;
    API_FIELD() static String Build;
    // Save loading
    API_FIELD() static String Character;
    API_FIELD() static String Universe;
    API_FIELD() static bool Autosave;
    API_FIELD() static String Level;

    void OnInitialize() override;

    void RegisterArguments();
};
