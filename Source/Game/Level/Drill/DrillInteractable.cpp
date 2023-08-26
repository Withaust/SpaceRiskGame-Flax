#include "DrillInteractable.h"

DrillInteractable::DrillInteractable(const SpawnParams& params)
    : IInteractable(params)
{

}

void DrillInteractable::OnClose(Entity* entity)
{
    GetEntity()->GetComponent<Drill>()->SetFuelRemote(0.0f);
}


void DrillInteractable::OnEnable()
{
    Function<void(Entity*)> func;
    func.Bind<DrillInteractable, &DrillInteractable::OnClose>(this);

    AddOption(InteractionType::Close, func);
}
