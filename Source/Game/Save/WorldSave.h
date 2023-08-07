#pragma once

#include <Engine/Scripting/Script.h>

API_CLASS() class GAME_API WorldSave : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(WorldSave);

    API_FIELD() String Name = TEXT("Default");
    API_FIELD() float Aggression = 0.0f;
};
