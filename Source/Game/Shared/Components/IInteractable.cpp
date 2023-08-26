#include "IInteractable.h"

InteractableOption IInteractable::DefaultOption;

IInteractable::IInteractable(const SpawnParams& params)
    : IComponent(params)
{
}

void IInteractable::AddOption(InteractionType type, Function<void(Entity*)> invoke, bool enabled, uint32 order)
{
    InteractableOption newOption;
    newOption.Type = type;
    newOption.Invoke = invoke;
    newOption.Enabled = enabled;
    
    if (order == 0)
    {
        _options.Add(newOption);
    }
    else
    {
        _options.Insert(order, newOption);
    }
}

InteractableOption& IInteractable::GetOption(InteractionType type)
{
    for (int i = 0; i < _options.Count(); ++i)
    {
        if (_options[i].Type == type)
        {
            return _options.At(i);
        }
    }
    return DefaultOption;
}

const Array<InteractableOption>& IInteractable::GetOptions()
{
    return _options;
}
