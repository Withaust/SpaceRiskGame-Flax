#pragma once

#include <Engine/Scripting/Script.h>

API_CLASS() class GAME_API CharacterSave : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(CharacterSave);

    API_FIELD() String Name = TEXT("Default");
    API_FIELD() float Experience = 0.0f;
};
