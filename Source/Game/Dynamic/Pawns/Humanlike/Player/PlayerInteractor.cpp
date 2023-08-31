#include "PlayerInteractor.h"

Delegate<IInteractable*> PlayerInteractor::OnShowMenu;
Delegate<IInteractable*> PlayerInteractor::OnHideMenu;

PlayerInteractor::PlayerInteractor(const SpawnParams& params)
    : IComponent(params)
{
    _tickUpdate = true;
}

void PlayerInteractor::OnEnable()
{
    _sleep = 5.0f;
}

void PlayerInteractor::ExecuteOption(InteractableOption& option)
{
    if (!_target)
    {
        return;
    }

    option.Invoke(GetEntity());
}

void PlayerInteractor::Show()
{
    if (!_showing)
    {
        _showing = true;
        ASSERT(_target);
        OnShowMenu(_target);
    }
}

void PlayerInteractor::Hide()
{
    if (_showing)
    {
        _showing = false;
        ASSERT(_target);
        OnHideMenu(_target);
    }
}

void PlayerInteractor::OnUpdate()
{
    UNOT_OWNED_RETURN();

    if (!USLEEP(_sleep))
    {
        return;
    }

    RayCastHit hit;

    if (!Physics::RayCast(RaycastOrigin->GetPosition(), RaycastOrigin->GetDirection(), hit, RaycastDistance, RaycastMask, false))
    {
        Hide();
        return;
    }

    Actor* actor = Cast<Actor>(hit.Collider);

    if (!actor)
    {
        Hide();
        return;
    }

    Entity* entity = Entity::FindEntity(actor);
    
    if (!entity)
    {
        Hide();
        return;
    }

    IInteractable* interactable = entity->GetComponent<IInteractable>();

    if (!interactable || !interactable->IsEnabled())
    {
        Hide();
        return;
    }

    if (interactable != _target)
    {
        _target = interactable;
        Hide();
    }

    Show();
}
