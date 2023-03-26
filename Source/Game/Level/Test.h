#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/Util/Defines.h>

API_CLASS() class GAME_API Test : public Script
{
API_AUTO_SERIALIZATION();
DECLARE_SCRIPTING_TYPE(Test);

private:

    SleepBlock Example;

public:

    void OnEnable() override;
    void OnDisable() override;
    void OnUpdate() override;
};
