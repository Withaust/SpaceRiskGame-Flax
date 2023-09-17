#include "UIPlayMenuMap.h"

UIPlayMenuMap::UIPlayMenuMap(const SpawnParams& params)
    : IUIUnit(params)
{
    _tickUpdate = true;
}

void UIPlayMenuMap::OnInitialize()
{

}

void UIPlayMenuMap::OnDeinitialize()
{

}

void UIPlayMenuMap::OnEnable()
{
    if (Map::Instance && Map::Instance->Camera)
    {
        _camera = Map::Instance->Camera;
        _camera->SetIsActive(true);
        _distance = -((Map::Instance->MaxZoom - Map::Instance->MinZoom) / 2.0f);
        _camera->SetLocalPosition(Vector3(0.0f, 0.0f, _distance));
        _camera->GetParent()->SetOrientation(Quaternion::Euler(45.0f, 0.0f, 0.0f));
        _camera->LookAt(GetParent()->GetPosition(), Vector3::Up);
    }
}

void UIPlayMenuMap::OnDisable()
{
    if (_camera)
    {
        _camera->SetIsActive(false);
    }
}

void UIPlayMenuMap::OnUpdate()
{
    
    if (Input::GetMouseButton(MouseButton::Right))
    {
        // Get mouse axis values and clamp pitch
        _yaw += Input::GetAxis(TEXT("Mouse X")) * _mouseSensitivity * Time::GetDeltaTime(); // H
        _pitch += Input::GetAxis(TEXT("Mouse Y")) * _mouseSensitivity * Time::GetDeltaTime(); // V
        _pitch = Math::Clamp(_pitch, _pitchMin, _pitchMax);

        if (_camera)
        {
            _camera->GetParent()->SetOrientation(Quaternion::Euler(_pitch, _yaw, 0));
            _camera->LookAt(GetParent()->GetPosition(), Vector3::Up);
        }
    }

    float scroll = Input::GetMouseScrollDelta();

    if (scroll != 0.0f)
    {
        _zoomStep = Math::Remap(_distance, -Map::Instance->MinZoom, -Map::Instance->MaxZoom, _zoomStepMin, _zoomStepMax);
        if (scroll > 0.0f)
        {
            _distance += _zoomStep;
        }
        else
        {
            _distance -= _zoomStep;
        }
        _distance = Math::Clamp(_distance, -Map::Instance->MaxZoom, -Map::Instance->MinZoom);
        _camera->SetLocalPosition(Vector3(0.0f, 0.0f, _distance));
    }

    if (Map::Instance)
    {
        Map::Instance->UpdateSizes();
    }
}
