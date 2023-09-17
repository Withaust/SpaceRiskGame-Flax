#pragma once

#include "Engine/Scripting/Script.h"

API_CLASS() class GAME_API UIPlayMenuInventory : public Script
{
API_AUTO_SERIALIZATION();
DECLARE_SCRIPTING_TYPE(UIPlayMenuInventory);

    // [Script]
    void OnEnable() override;
    void OnDisable() override;
    void OnUpdate() override;
};
