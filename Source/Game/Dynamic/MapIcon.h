#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/UI/SpriteRender.h>

#include <Game/System/Game/Map.h>

API_CLASS() class GAME_API MapIcon : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(MapIcon);
public:

    void OnEnable() override;
    void OnDisable() override;
};
