#include "CameraControl.h"

CameraControl::CameraControl(const SpawnParams& params)
    : IInteractable(params)
{
    _tickUpdate = true;
}

void CameraControl::OnToggle(Entity* entity)
{
    _state = true;
    entity->GetComponent<PlayerMovement>()->Camera->SetIsActive(false);
    Cameras[0]->SetIsActive(true);
    _index = 0;
    Disable();
}

void CameraControl::OnEnable()
{
    AddOption(InteractionType::Use, [this](Entity* e) -> void { this->OnToggle(e); });

    for (const auto& camera : Cameras)
    {
        camera->SetIsActive(false);
    }
}

void CameraControl::OnUpdate()
{
    if (!_state)
    {
        return;
    }

    if (Input::GetKeyDown(KeyboardKeys::Escape))
    {
        auto player = PlayerManager::Instance->GetOurPlayer();
        Cameras[_index]->SetIsActive(false);
        player->GetComponent<PlayerMovement>()->Camera->SetIsActive(true);
        _index = 0;
        Enable();
    }

    if (Input::GetKeyDown(KeyboardKeys::ArrowLeft))
    {
        Cameras[_index]->SetIsActive(false);
        _index--;

        if (_index < 0)
        {
            _index = Cameras.Count() - 1;
        }
        
        Cameras[_index]->SetIsActive(true);
    }
    else if (Input::GetKeyDown(KeyboardKeys::ArrowRight))
    {
        Cameras[_index]->SetIsActive(false);
        _index++;

        if (_index >= Cameras.Count())
        {
            _index = 0;
        }

        Cameras[_index]->SetIsActive(true);
    }
}