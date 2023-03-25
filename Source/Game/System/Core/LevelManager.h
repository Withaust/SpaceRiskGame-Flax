#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Content/Content.h>

#include <Game/System/Core/CoreInstance.h>
#include <Game/System/Core/Logger.h>

API_CLASS() class GAME_API LevelManager : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(LevelManager);
    USYSTEM(LevelManager);

public:

    void OnInitialize() override;
    void OnDeinitialize() override;

    void LoadLevel(String scene);
};
