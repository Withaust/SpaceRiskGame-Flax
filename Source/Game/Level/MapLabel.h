#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/System/Game/Map.h>

API_CLASS() class GAME_API MapLabel : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(MapLabel);
public:

    void OnEnable() override;
    void OnDisable() override;
};
