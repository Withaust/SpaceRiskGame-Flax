#include "DrillInteractable.h"

DrillInteractable::DrillInteractable(const SpawnParams& params)
    : IInteractable(params)
{

}

void DrillInteractable::OnClose(Entity* entity)
{
    Drill* drill = GetEntity()->GetComponent<Drill>();
    Chat::Instance->SendMessage(String::Format(TEXT("Collected {0} fuel"), drill->GetFuelLocal()));
    drill->SetFuelRemote(0.0f);
}


void DrillInteractable::OnEnable()
{
    Function<void(Entity*)> func;
    func.Bind<DrillInteractable, &DrillInteractable::OnClose>(this);

    AddOption(InteractionType::Collect, [this](Entity* e) -> void { this->OnClose(e); }, true, 1.0f);
}
