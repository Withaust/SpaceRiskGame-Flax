#include "PlayerMovement.h"

float PlayerMovement::Height = 50.0f;
float PlayerMovement::Radius = 25.0f;

PlayerMovement::PlayerMovement(const SpawnParams& params)
    : IComponent(params)
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

Vector3 PlayerMovement::ClampHorizontal(Vector3 velocity)
{
    return Vector3(velocity.X, 0.0f, velocity.Z);
}

Vector3 PlayerMovement::ClampNonHorizontal(Vector3 velocity)
{
    return Vector3(0.0f, velocity.Y, 0.0f);
}

void PlayerMovement::OnUpdate()
{
    UNOT_OWNED_RETURN();

    bool jump = false;

    if (CanMove && !Screen::GetCursorVisible())
    {
        // Movement
        _horizontal += Input::GetAxis(TEXT("Horizontal"));
        _vertical += Input::GetAxis(TEXT("Vertical"));

        // Mouse
        Float2 mouseDelta(Input::GetAxis(TEXT("Mouse X")), Input::GetAxis(TEXT("Mouse Y")));
        _pitch = Math::Clamp(_pitch + mouseDelta.Y, -89.0f, 89.0f);
        _yaw += mouseDelta.X;

        // Jump
        jump = CanJump && Input::GetKey(KeyboardKeys::Spacebar); //Input::GetAction(TEXT("Jump"));
    }

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

    if (Controller->IsGrounded())
    {
        velocity = MoveGround(velocity, ClampHorizontal(_velocity));
        velocity.Y = -Math::Abs(Physics::GetGravity().Y * 0.5f);
    }
    else
    {
        velocity = MoveAir(velocity, ClampHorizontal(_velocity));
        velocity.Y = _velocity.Y;
    }

    // Fix direction
    if (velocity.Length() < 0.05f)
    {
        velocity = Vector3::Zero;
    }

    // Jump
    if (jump && Controller->IsGrounded())
    {
        velocity.Y = JumpForce;
    }

    // Apply gravity
    velocity.Y += -Math::Abs(Physics::GetGravity().Y * 2.5f) * Time::GetDeltaTime();

    // Check if player is not blocked by something above head
    if (EnumHasAnyFlags(Controller->GetFlags(), CharacterController::CollisionFlags::Above))
    {
        if (velocity.Y > 0.0f)
        {
            // Player head hit something above, zero the gravity acceleration
            velocity.Y = 0.0f;
        }
    }

    velocity.X = Math::Clamp(velocity.X, -MaxVelocityClamp, MaxVelocityClamp);
    velocity.Y = Math::Clamp(velocity.Y, -MaxVelocityClamp, MaxVelocityClamp);
    velocity.Z = Math::Clamp(velocity.Z, -MaxVelocityClamp, MaxVelocityClamp);

    // Move
    Controller->AddMovement(velocity * Time::GetDeltaTime(), Quaternion::Identity);
    _velocity = velocity;
}
