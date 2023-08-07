#pragma once

#include <Engine/Scripting/Script.h>

API_CLASS() class GAME_API HumanlikeVisual : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(HumanlikeVisual);

    void OnEnable() override;
    void OnDisable() override;
    void OnUpdate() override;
};
