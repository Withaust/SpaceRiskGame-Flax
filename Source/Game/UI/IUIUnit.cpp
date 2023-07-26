#include "IUIUnit.h"

IUIUnit::IUIUnit(const SpawnParams& params)
    : Script(params)
{
}

RmlUiDocument* IUIUnit::GetDocument()
{
    return Cast<RmlUiDocument>(GetParent());
}
