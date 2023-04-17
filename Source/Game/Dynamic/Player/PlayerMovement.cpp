#include "PlayerMovement.h"

PlayerMovement::PlayerMovement(const SpawnParams& params)
    : Script(params),
    Print(0.5f)
{
    _tickUpdate = true;
}

Vector3 PlayerMovement::Accelerate(Vector3 accelDir, Vector3 prevVelocity, float accelerate, float maxVelocity)
{
    float projVel = Vector3::Dot(prevVelocity, accelDir); // Vector projection of Current velocity onto accelDir
    float accelVel = accelerate * Time::GetDeltaTime(); // Accelerated velocity in direction of movement

    // If necessary, truncate the accelerated velocity so the vector projection does not exceed max velocity
    if (projVel + accelVel > maxVelocity)
    {
        accelVel = maxVelocity - projVel;
    }

    return prevVelocity + accelDir * accelVel;
}

Vector3 PlayerMovement::MoveGround(Vector3 accelDir, Vector3 prevVelocity)
{
    // Apply Friction
    float speed = prevVelocity.Length();
    if (Math::Abs(speed) > 0.01f) // To avoid divide by zero errors
    {
        float drop = speed * Friction * Time::GetDeltaTime();
        prevVelocity *= Math::Max(speed - drop, 0.0f) / speed; // Scale the velocity based on friction
    }

    // GroundAccelerate and MaxVelocityGround are server-defined movement variables
    return Accelerate(accelDir, prevVelocity, GroundAccelerate, MaxVelocityGround);
}

Vector3 PlayerMovement::MoveAir(Vector3 accelDir, Vector3 prevVelocity)
{
    // air_accelerate and max_velocity_air are server-defined movement variables
    return Accelerate(accelDir, prevVelocity, AirAccelerate, MaxVelocityAir);
}

Vector3 PlayerMovement::Horizontal(Vector3 v)
{
    return Vector3(v.X, 0.0f, v.Z);
}

void PlayerMovement::OnEnable()
{
}

void PlayerMovement::OnDisable()
{
}

void PlayerMovement::OnUpdate()
{
    UNOT_OWNED_RETURN;

    // Movement
    _horizontal += Input::GetAxis(TEXT("Horizontal"));
    _vertical += Input::GetAxis(TEXT("Vertical"));

    // Mouse
    Float2 mouseDelta(Input::GetAxis(TEXT("Mouse X")), Input::GetAxis(TEXT("Mouse Y")));
    _pitch = Math::Clamp(_pitch + mouseDelta.Y, -89.0f, 89.0f);
    _yaw += mouseDelta.X;

    // Jump
    bool jump = CanJump && Input::GetKey(KeyboardKeys::Spacebar); //Input::GetAction(TEXT("Jump"));

    // Update head
    Transform headTrans = Head->GetLocalTransform();
    headTrans.Orientation = Quaternion::Euler(_pitch, _yaw, 0.0f);
    Head->SetLocalTransform(headTrans);

    // Calculate player movement vector
    Vector3 velocity(_horizontal, 0.0f, _vertical);
    _horizontal = 0.0f;
    _vertical = 0.0f;
    velocity.Normalize();
    Vector3 rotation = Head->GetOrientation().GetEuler();
    rotation.X = 0.0f;
    rotation.Z = 0.0f;
    velocity = Vector3::Transform(velocity, Quaternion::Euler(rotation));
    velocity.Normalize();
    if (PlayerController->IsGrounded())
    {
        velocity = MoveGround(velocity, Horizontal(_velocity));
        velocity.Y = -Math::Abs(Physics::GetGravity().Y * 0.5f);
    }
    else
    {
        velocity = MoveAir(velocity, Horizontal(_velocity));
        velocity.Y = _velocity.Y;
    }

    // Fix direction
    if (velocity.Length() < 0.05f)
    {
        velocity = Vector3::Zero;
    }

    // Jump
    if (jump && PlayerController->IsGrounded())
    {
        velocity.Y = JumpForce;
    }

    // Apply gravity
    velocity.Y += -Math::Abs(Physics::GetGravity().Y * 2.5f) * Time::GetDeltaTime();

    // Check if player is not blocked by something above head
    if ((static_cast<int>(PlayerController->GetFlags()) & static_cast<int>(CharacterController::CollisionFlags::Above)) != 0)
    {
        if (velocity.Y > 0.0f)
        {
            // Player head hit something above, zero the gravity acceleration
            velocity.Y = 0.0f;
        }
    }

    // Move
    PlayerController->AddMovement(velocity * Time::GetDeltaTime(), Quaternion::Identity);
    _velocity = velocity;
}
