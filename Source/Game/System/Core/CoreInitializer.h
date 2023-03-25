#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Level.h>

#include <Game/System/Core/CoreInstance.h>
#include <Game/System/Core/LevelManager.h>

API_CLASS() class GAME_API CoreInitializer : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(CoreInitializer);
    USYSTEM(CoreInitializer);
public:

    void OnInitialize() override;
    void OnDeinitialize() override;
};
