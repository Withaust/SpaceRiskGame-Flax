#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Core/Math/Vector3.h>
#include <Engine/Engine/Time.h>
#include <Engine/Engine/Engine.h>
#include <Engine/Input/Input.h>
#include <Engine/Engine/Screen.h>
#include <Engine/Level/Actors/StaticModel.h>
#include <Engine/Level/Actors/Camera.h>
#include <Engine/Physics/Colliders/CharacterController.h>
#include <Engine/Physics/Physics.h>
#include <Engine/Debug/DebugDraw.h>

#include <Game/Utils/Defines.h>

API_CLASS() class GAME_API PlayerMovement : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerMovement);

public:

    API_FIELD() ScriptingObjectReference<CharacterController> PlayerController;
    API_FIELD() ScriptingObjectReference<Camera> Camera;

    API_FIELD() float Friction = 8.0f;
    API_FIELD() float GroundAccelerate = 5000.0f;
    API_FIELD() float AirAccelerate = 10000.0f;
    API_FIELD() float MaxVelocityGround = 400.0f;
    API_FIELD() float MaxVelocityAir = 200.0f;
    API_FIELD() bool UseMouse = true;
    API_FIELD() bool CanJump = true;
    API_FIELD() bool CanShoot = true;
    API_FIELD() float JumpForce = 800.0f;
    API_FIELD() float CameraSmoothing = 20.0f;

private:

    Vector3 _velocity;
    float _pitch = 0.0f;
    float _yaw = 0.0f;
    float _horizontal = 0.0f;
    float _vertical = 0.0f;

    // accelDir: normalized direction that the player has requested to move (taking into account the movement keys and look direction)
    // prevVelocity: The current velocity of the player, before any additional calculations
    // accelerate: The server-defined player acceleration value
    // maxVelocity: The server-defined maximum player velocity (this is not strictly adhered to due to strafejumping)
    Vector3 Accelerate(Vector3 accelDir, Vector3 prevVelocity, float accelerate, float maxVelocity);
    Vector3 MoveGround(Vector3 accelDir, Vector3 prevVelocity);
    Vector3 MoveAir(Vector3 accelDir, Vector3 prevVelocity);
    Vector3 Horizontal(Vector3 v);

public:

    void OnUpdate() override;
};
