#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Level.h>

#include <Game/Shared/Defines.h>
#include <Game/System/Core/LaunchArgs.h>
#include <Game/System/Core/LevelManager.h>
#include <Game/System/Core/Networking.h>
#include <Game/System/Core/UI.h>

API_CLASS() class GAME_API CoreInitializer : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(CoreInitializer);
public:

    void OnInitialize() override;
    void OnDeinitialize() override;
    void OnSceneLoaded(Scene* scene) override;
};
