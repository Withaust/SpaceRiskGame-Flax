#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/System/Game/Map.h>

API_CLASS() class GAME_API MapCamera : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(MapCamera);

private:

    bool _everDisabled = false;

public:

    void OnEnable() override;
    void OnDisable() override;
};
