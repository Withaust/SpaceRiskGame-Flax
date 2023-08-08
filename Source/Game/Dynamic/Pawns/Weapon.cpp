#include "Weapon.h"

Weapon::Weapon(const SpawnParams& params)
    : IComponent(params),
    _batchBlock(5.0f)
{
    _tickUpdate = true;
}

void Weapon::OnEnable()
{
    UBIND_DATA(Weapon, Data);
    BulletsEffect->SetParameterValue(TEXT("Main"), TEXT("Rate"), (1000.0f / DataPtr->FireRate) - 1.0f);
    _info = GetEntity()->GetComponent<ImmediateInfo>();
}

void Weapon::OnUpdate()
{
    if (UOWNED)
    {
        _nextShoot = Math::Clamp(_nextShoot - Time::GetDeltaTime(), -1.0f, DataPtr->FireRate);
        bool hasShot = DataPtr->Automatic ? Input::GetMouseButton(MouseButton::Left) : Input::GetMouseButtonDown(MouseButton::Left);
        if (_nextShoot <= 0.0f)
        {
            if (hasShot)
            {
                Vector3 spread = ShootForHurt();
                ShootForVisual(spread);
                _nextShoot = DataPtr->FireRate;
                if (!_firedBefore)
                {
                    _firedBefore = true;
                    _info->Buttons |= ImmediateInfo::ButtonsMask::AttackLeft;
                }
            }
            else
            {
                BulletsEffect->SetParameterValue(TEXT("Main"), TEXT("Active"), false);
                if (_firedBefore)
                {
                    _firedBefore = false;
                    _info->Buttons &= ~ImmediateInfo::ButtonsMask::AttackLeft;
                }
            }
        }
    }
    else
    {
        if (EnumHasAllFlags(_info->Buttons, ImmediateInfo::ButtonsMask::AttackLeft))
        {
            ShootForVisual();
        }
        else
        {
            BulletsEffect->SetParameterValue(TEXT("Main"), TEXT("Active"), false);
        }
    }
}

void Weapon::ShootForVisual(Vector3 Spread)
{
    if (Spread == Vector3::Zero)
    {
        RandomStream random;
        random.GenerateNewSeed();
        float spreadX = random.RandRange(-DataPtr->Spread, DataPtr->Spread);
        float spreadY = random.RandRange(-DataPtr->Spread, DataPtr->Spread);
        float spreadZ = random.RandRange(-DataPtr->Spread, DataPtr->Spread);
        Spread = Vector3(spreadX, spreadY, spreadZ);
    }

    BulletsEffect->LookAt((Muzzle->GetPosition() + (Direction->GetDirection() * DataPtr->Distance)) + Spread);
    BulletsEffect->SetParameterValue(TEXT("Main"), TEXT("Active"), true);
}

Vector3 Weapon::ShootForHurt()
{
    RandomStream random;
    random.GenerateNewSeed();
    float spreadX = random.RandRange(-DataPtr->Spread, DataPtr->Spread);
    float spreadY = random.RandRange(-DataPtr->Spread, DataPtr->Spread);
    float spreadZ = random.RandRange(-DataPtr->Spread, DataPtr->Spread);
    Vector3 spread(spreadX, spreadY, spreadZ);

    DEBUG_DRAW_LINE(Direction->GetPosition(), (Direction->GetPosition() + (Direction->GetDirection() * DataPtr->Distance)) + spread, Color::Orange, 1.0f, true);

    return spread;
}