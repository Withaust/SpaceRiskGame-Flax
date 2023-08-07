#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Serialization/JsonWriters.h>
#include <Engine/Serialization/JsonSerializer.h>
#include <Engine/Content/Content.h>
#include <Engine/Platform/File.h>

#include <Game/Shared/Defines.h>
#include <Game/UI/IUIState.h>
#include <Game/System/Core/LaunchArgs.h>
#include <Game/Save/CharacterSave.h>
#include <Game/Save/WorldSave.h>


API_CLASS() class GAME_API Saver : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Saver);

private:

    ScriptingObjectReference<Args> _args;
    
    static const Char* _characterPath;
    static const Char* _worldPath;

    void LoadCharacter();
    void LoadWorld();
    

public:
    API_FIELD() static Saver* Instance;

    API_FIELD() ScriptingObjectReference<CharacterSave> Character;
    API_FIELD() ScriptingObjectReference<WorldSave> World;

    API_FUNCTION() static void SaveDefaultCharacter();
    API_FUNCTION() static void SaveDefaultWorld();

    API_FUNCTION() void SaveCharacterAs(String Name);
    API_FUNCTION() void SaveWorldAs(String Name);

    API_FUNCTION() void SaveCharacter();
    API_FUNCTION() void SaveWorld();

    void OnInitialize() override;
    void OnDeinitialize() override;
};
