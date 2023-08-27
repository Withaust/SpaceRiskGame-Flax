#include "Button.h"

Button::Button(const SpawnParams& params)
    : IInteractable(params)
{

}

void Button::SetStateLocal(bool value)
{
    _state = value;

    for (const auto& light : Lights)
    {
        light->SetIsActive(_state);
    }
}

void Button::OnToggle(Entity* entity)
{
    SetStateRemote(!GetStateLocal());
}

void Button::OnEnable()
{
    Function<void(Entity*)> func;
    func.Bind<Button, &Button::OnToggle>(this);

    AddOption(InteractionType::Toggle, func);
}
