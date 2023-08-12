#include "Weapon.h"

Weapon::Weapon(const SpawnParams& params)
    : IComponent(params),
    _batchBlock(5.0f)
{
    _tickUpdate = true;
}

float Weapon::ProcessHurt(const Array<RayCastHit>& result)
{
    RandomStream random;
    random.GenerateNewSeed();
    float Damage = random.RandRange(DataPtr->Damage - DataPtr->DamageSpread, DataPtr->Damage + DataPtr->DamageSpread);

    for (int i = 0; i < result.Count(); ++i)
    {
        const auto& hit = result.At(i);
        const auto& hitActor = hit.Collider;

        bool found = false;
        if (IDamagePenetratable* penetration = hitActor->GetScript<IDamagePenetratable>())
        {
            found = true;
            Damage = penetration->InflictPenetration(GetEntity(), Damage);
            if (Damage <= 0.0f)
            {
                break;
            }
        }
        if (IHitBox* hitbox = hitActor->GetScript<IHitBox>())
        {
            found = true;
            Guid entity = Networking::Instance->SerializeEntity(GetEntity());
            Entity::FindEntity(hitActor)->GetComponent<IDamage>()->InflictDamageServer(hitbox->GetHitBoxIndex(), entity, Damage);
        }
        if (!found)
        {
            break;
        }
    }

    if (result.Count() == 0)
    {
        return result[0].Distance;
    }
    else
    {
        return 0.0f;
    }

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

void Weapon::ShootForVisual(Vector3 Spread, float Distance)
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
    if (Distance == 0.0f)
    {
        if (Physics::RayCast(Muzzle->GetPosition(), Muzzle->GetDirection(), visualResult, DataPtr->Distance, DamageMask, false))
        {
            Distance = visualResult.Distance;
        }
        else
        {
            Distance = DataPtr->Distance;
        }
    }

    BulletsEffect->LookAt((Muzzle->GetPosition() + (Direction->GetDirection() * DataPtr->Distance)) + Spread);
    BulletsEffect->SetParameterValue(TEXT("Main"), TEXT("Lifetime"), (Distance - BulletMuzzleOffset) / DataPtr->Velocity);
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

    Vector3 start = Direction->GetPosition();
    Vector3 direction = Direction->GetDirection();

    DEBUG_DRAW_LINE(start, (Direction->GetPosition() + (Direction->GetDirection() * DataPtr->Distance)) + spread, Color::Orange, 1.0f, true);

    results.Clear();
    if (Physics::RayCastAll(start, direction, results, DataPtr->Distance, DamageMask, false))
    {
        ProcessHurt(results);
    }

    return spread;
}