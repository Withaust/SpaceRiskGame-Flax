#include "Drill.h"

Drill::Drill(const SpawnParams& params)
    : IComponent(params),
    Generate(4.0f)
{
    _tickUpdate = true;
}

float Drill::GetFuel() const
{
    return _fuel;
}

void Drill::SetFuel(float value)
{
    _fuel = Math::Clamp(value, 0.0f, DataPtr->MaxFuel);
    Text->SetText(String::Format(TEXT("{0}\n{1}"), static_cast<int>(_fuel), DataPtr->Name));
}

void Drill::OnEnable()
{
    UBIND_DATA(Drill, Data);
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

    USLEEP(Generate)
    {
        SetFuel(_fuel + DataPtr->GenerateSpeed);
    }
}
