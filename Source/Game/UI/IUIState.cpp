#include "IUIState.h"

IUIState::IUIState(const SpawnParams& params)
    : Script(params)
{
}

void IUIState::OnInitialize()
{
    for (const auto& script : GetParent()->Scripts)
    {
        auto unit = Cast<IUIUnit>(script);
        if (unit == nullptr)
        {
            continue;
        }
        unit->OnInitialize();
        _units.Add(unit);
    }
}

void IUIState::OnDeinitialize()
{
    for (int i = _units.Count() - 1; i >= 0; i--)
    {
        _units[i]->OnDeinitialize();
    }
}

RmlUiDocument* IUIState::GetDocument()
{
    return Cast<RmlUiDocument>(GetParent());
}
