#include "Drill.h"

Drill::Drill(const SpawnParams& params)
    : IComponent(params)
{
    _tickUpdate = true;
}

void Drill::SetFuelLocal(float value)
{
    _fuel = Math::Clamp(value, 0.0f, DataPtr->MaxFuel);
    Text->SetText(String::Format(TEXT("{0}\n{1}"), static_cast<int>(_fuel), DataPtr->Name));
}

void Drill::OnEnable()
{
    Generate = 4.0f;
    UBIND_DATA(Drill, Data);
}

void Drill::OnUpdate()
{
    if (_startToEnd)
    {
        Head->SetPosition(Vector3::Lerp(Head->GetPosition(), End->GetPosition(), (DataPtr->GenerateSpeed / 100.0f) * Time::GetDeltaTime()));

        if (Vector3::Distance(Head->GetPosition(), End->GetPosition()) < 1.0f)
        {
            _startToEnd = false;
        }
    }
    else
    {
        Head->SetPosition(Vector3::Lerp(Head->GetPosition(), Start->GetPosition(), (DataPtr->GenerateSpeed / 100.0f) * Time::GetDeltaTime()));

        if (Vector3::Distance(Head->GetPosition(), Start->GetPosition()) < 1.0f)
        {
            _startToEnd = true;
        }
    }

    if(USLEEP(Generate))
    {
        SetFuelLocal(_fuel + DataPtr->GenerateSpeed);
    }
}
