#include "PlayerInteractor.h"

Delegate<IInteractable*> PlayerInteractor::OnShowMenu;
Delegate<> PlayerInteractor::OnHideMenu;
Delegate<float> PlayerInteractor::OnShowProgress;
Delegate<float> PlayerInteractor::OnUpdateProgress;
Delegate<> PlayerInteractor::OnHideProgress;

PlayerInteractor::PlayerInteractor(const SpawnParams& params)
    : IComponent(params)
{
    _tickUpdate = true;
}

void PlayerInteractor::OnEnable()
{
    _sleep = 20.0f;
}

void PlayerInteractor::StartInteract(InteractableOption& option)
{
    if (_interacting)
    {
        return;
    }
    _option = option;
    if (_option.Timer == 0.0f)
    {
        _option.Invoke(GetEntity());
    }
    else
    {
        _interacting = true;
        _timer = option.Timer;
        _currentTimer = 0.0f;
        OnShowProgress(_timer);
    }
}

void PlayerInteractor::StopInteract()
{
    if (_interacting)
    {
        _interacting = false;
        _timer = 0.0f;
        _currentTimer = 0.0f;
        OnHideProgress();
    }
}

void PlayerInteractor::Show()
{
    if (_interacting)
    {
        _showing = false;
        OnHideMenu();
    }
    else 
    {
        if (!_showing)
        {
            _showing = true;
            ASSERT(_target);
            OnShowMenu(_target);
        }
    }
}

void PlayerInteractor::Hide()
{
    if (_interacting)
    {
        StopInteract();
    }
    if (_showing)
    {
        _showing = false;
        OnHideMenu();
    }
}

void PlayerInteractor::OnUpdate()
{
    UNOT_OWNED_RETURN();

    // Update is done every frame, otherwise progress bar is not smooth enough
    if (_interacting)
    {
        _currentTimer += Time::GetDeltaTime();
        if (_currentTimer >= _timer)
        {
            StopInteract();
            _option.Invoke(GetEntity());
        }
        else
        {
            OnUpdateProgress(_currentTimer);
        }
    }

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
