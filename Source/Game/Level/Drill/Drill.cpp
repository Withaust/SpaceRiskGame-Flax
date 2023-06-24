#include "Drill.h"

Drill::Drill(const SpawnParams& params)
    : IComponent(params),
    Generate(1.0f)
{
    _tickUpdate = true;
}

float Drill::GetFuel() const
{
    return _fuel;
}

void Drill::SetFuel(float value)
{
    Text->SetText(String::Format(TEXT("{0}"), static_cast<int>(value)));
    _fuel = value;
}

void Drill::OnEnable()
{

}

void Drill::OnUpdate()
{
    if (_startToEnd)
    {
        Head->SetPosition(Vector3::Lerp(Head->GetPosition(), End->GetPosition(), 5.0f * Time::GetDeltaTime()));

        if (Vector3::Distance(Head->GetPosition(), End->GetPosition()) < 1.0f)
        {
            _startToEnd = false;
        }
    }
    else
    {
        Head->SetPosition(Vector3::Lerp(Head->GetPosition(), Start->GetPosition(), 5.0f * Time::GetDeltaTime()));

        if (Vector3::Distance(Head->GetPosition(), Start->GetPosition()) < 1.0f)
        {
            _startToEnd = true;
        }
    }

    UNOT_OWNED_RETURN();

    USLEEP(Generate)
    {
        SetFuel(_fuel + GenerateSpeed);
    }
}
