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

API_CLASS() class GAME_API PlayerMovement : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerMovement);

public:

    API_FIELD(Attributes = "EditorDisplay(\"Movement\")");
    float Friction = 8.0f;

    API_FIELD(Attributes = "EditorDisplay(\"Movement\")");
    float GroundAccelerate = 5000.0f;

    API_FIELD(Attributes = "EditorDisplay(\"Movement\")");
    float AirAccelerate = 10000.0f;

    API_FIELD(Attributes = "EditorDisplay(\"Movement\")");
    float MaxVelocityGround = 400.0f;

    API_FIELD(Attributes = "EditorDisplay(\"Movement\")");
    float MaxVelocityAir = 200.0f;

    API_FIELD(Attributes = "EditorDisplay(\"Movement\")");
    ScriptingObjectReference<CharacterController> Controller;

    API_FIELD(Attributes = "EditorDisplay(\"Input\")");
    bool CanJump = true;

    API_FIELD(Attributes = "EditorDisplay(\"Input\")");
    bool CanShoot = true;

    API_FIELD(Attributes = "EditorDisplay(\"Input\")");
    float JumpForce = 800.0f;

    API_FIELD(Attributes = "EditorDisplay(\"Visual\")");
    ScriptingObjectReference<StaticModel> VisualMesh;

    API_FIELD(Attributes = "EditorDisplay(\"Visual\")");
    ScriptingObjectReference<Camera> Camera;

private:

    Vector3 _velocity;
    float _pitch = 0.0f;
    float _yaw = 0.0f;
    float _horizontal = 0.0f;
    float _vertical = 0.0f;

    /// <summary>
    /// Adds the movement and rotation to the camera (as input).
    /// </summary>
    /// <param name="horizontal">The horizontal input.</param>
    /// <param name="vertical">The vertical input.</param>
    /// <param name="pitch">The pitch rotation input.</param>
    /// <param name="yaw">The yaw rotation input.</param>
    void AddMovementRotation(float horizontal, float vertical, float pitch, float yaw);

    // accelDir: normalized direction that the player has requested to move (taking into account the movement keys and look direction)
    // prevVelocity: The current velocity of the player, before any additional calculations
    // accelerate: The server-defined player acceleration value
    // maxVelocity: The server-defined maximum player velocity (this is not strictly adhered to due to strafejumping)
    Vector3 Accelerate(Vector3 accelDir, Vector3 prevVelocity, float accelerate, float maxVelocity);

    Vector3 MoveGround(Vector3 accelDir, Vector3 prevVelocity);

    Vector3 MoveAir(Vector3 accelDir, Vector3 prevVelocity);

    Vector3 Horizontal(Vector3 v);

public:

    void OnEnable() override;
    void OnDisable() override;
    void OnUpdate() override;
};
