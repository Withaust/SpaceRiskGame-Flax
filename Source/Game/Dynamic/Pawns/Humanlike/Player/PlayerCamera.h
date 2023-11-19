#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actors/Camera.h>

#include <Game/Shared/Defines.h>
#include <Game/Dynamic/Pawns/Humanlike/Player/PlayerMovement.h>

API_CLASS() class GAME_API PlayerCamera : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerCamera);

private:

    ScriptingObjectReference<PlayerMovement> _movement;

public:

    void OnEnable() override;
    void OnDisable() override;
};
