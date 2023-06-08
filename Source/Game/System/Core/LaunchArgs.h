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

API_CLASS() class GAME_API Args : public ScriptingObject, public ISerializable
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Args);
public:
    // Steam
    API_FIELD() bool IsSteam = false;
    API_FIELD() String Name;
    // Networking
    API_FIELD() bool IsHost = true;
    API_FIELD() String Hostname;
    API_FIELD() int Port = 40808;
    // Mod support
    API_FIELD() bool IsModded = false;
    API_FIELD() String Core;
};

API_CLASS() class GAME_API LaunchArgs : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(LaunchArgs);
private:

    ScriptingObjectReference<Args> _args;
    String GetNextArgument(int currentIndex, const Array<String>& args);

public:
    void OnInitialize() override;
    API_FUNCTION() const ScriptingObjectReference<Args> GetArgs();
};
