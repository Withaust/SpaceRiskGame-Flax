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
#include <Engine/Scripting/Enums.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/IComponent.h>

API_CLASS() class GAME_API PlayerMovement : public IComponent
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerMovement);

public:

    API_FIELD() static float Height;
    API_FIELD() static float Radius;

    API_FIELD() ScriptingObjectReference<CharacterController> Controller;
    API_FIELD() ScriptingObjectReference<Actor> Head;

    API_FIELD() float Friction = 8.0f;
    API_FIELD() float GroundAccelerate = 3000.0f;
    API_FIELD() float AirAccelerate = 8000.0f;
    API_FIELD() float MaxVelocityGround = 400.0f;
    API_FIELD() float MaxVelocityAir = 100.0f;
    API_FIELD() float MaxVelocityClamp = 3000.0f;
    API_FIELD() bool CanJump = true;
    API_FIELD() bool CanMove = true;
    API_FIELD() float JumpForce = 800.0f;
    API_FIELD() int FreezeFrames = 3;

private:

    int _freezeCounter = 0;
    bool _respawned = false;
    Vector3 _velocity;
    float _pitch = 0.0f;
    float _yaw = 0.0f;
    float _horizontal = 0.0f;
    float _vertical = 0.0f;


    // AccelDir: normalized direction that the player has requested to move (taking into account the movement keys and look direction)
    // PrevVelocity: The current velocity of the player, before any additional calculations
    // Accelerate: The server-defined player acceleration value
    // MaxVelocity: The server-defined maximum player velocity (this is not strictly adhered to due to strafejumping)
    Vector3 Accelerate(Vector3 accelDir, Vector3 prevVelocity, float accelerate, float maxVelocity);
    Vector3 MoveGround(Vector3 accelDir, Vector3 prevVelocity);
    Vector3 MoveAir(Vector3 accelDir, Vector3 prevVelocity);
    Vector3 ClampHorizontal(Vector3 velocity);
    Vector3 ClampNonHorizontal(Vector3 velocity);

public:

    void OnUpdate() override;
};
