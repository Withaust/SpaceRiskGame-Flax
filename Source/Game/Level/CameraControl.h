#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actor.h>
#include <Engine/Level/Actors/Camera.h>
#include <Engine/Input/Input.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/Components/IInteractable.h>
#include <Game/Dynamic/Pawns/Humanlike/Player/PlayerNetworking.h>

API_CLASS() class GAME_API CameraControl : public IInteractable
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(CameraControl);

private:

    void OnToggle(Entity* entity);

    bool _state = false;
    int _index = 0;

public:

    API_FIELD() Array<ScriptingObjectReference<Camera>> Cameras;

    void OnEnable() override;
    void OnUpdate() override;
};
