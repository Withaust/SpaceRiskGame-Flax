#include "CameraControl.h"

CameraControl::CameraControl(const SpawnParams& params)
    : IInteractable(params)
{
    _tickUpdate = true;
}

void CameraControl::OnToggle(Entity* entity)
{
    _state = true;
    PostFx::Instance->PovFx->SetEnabled(false);
    entity->GetComponent<PlayerNetworking>()->Camera->SetIsActive(false);
    entity->GetComponent<PlayerMovement>()->CanMove = false;
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
        Cameras[_index]->SetIsActive(false);
        auto player = PlayerManager::Instance->GetOurPlayer();
        player->GetComponent<PlayerNetworking>()->Camera->SetIsActive(true);
        player->GetComponent<PlayerMovement>()->CanMove = true;
        _index = 0;
        PostFx::Instance->PovFx->SetEnabled(true);
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