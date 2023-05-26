#pragma once

#include <Engine/Scripting/Script.h>
#include <Game/Shared/Defines.h>

API_CLASS() class GAME_API ClassA : public Actor
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(ClassA);

public:

    API_FIELD() int Int;
    API_FIELD() float Float;
};

API_CLASS() class GAME_API ClassB : public ClassA
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(ClassB);

public:

    API_FIELD() bool Bool;
    API_FIELD() double Double;
};