#include "UIPlayMenuMap.h"

UIPlayMenuMap::UIPlayMenuMap(const SpawnParams& params)
    : IUIUnit(params),
    _targetPosition()
{
    _tickUpdate = true;
}

void UIPlayMenuMap::OnInitialize()
{
    GetDocument()->GetDocument()->AddEventListener(Rml::EventId::Click, this);
}

void UIPlayMenuMap::OnDeinitialize()
{
    GetDocument()->GetDocument()->RemoveEventListener(Rml::EventId::Click, this);
}

void UIPlayMenuMap::OnEnable()
{
    if (Map::Instance && Map::Instance->Camera)
    {
        _camera = Map::Instance->Camera;
        _camera->SetIsActive(true);
        if (_targetDistance == 0.0f)
        {
            _targetDistance = -((Map::Instance->MaxZoom - Map::Instance->MinZoom) / 2.0f);
            _camera->LookAt(GetParent()->GetPosition(), Vector3::Up);
        }
        // Position
        _camera->GetParent()->SetPosition(_targetPosition);
        // Orientation
        _camera->GetParent()->SetOrientation(Quaternion::Euler(_targetPitch, 0.0f, 0.0f));
        // Distance
        _camera->SetLocalPosition(Vector3(0.0f, 0.0f, _targetDistance));
        Map::Instance->StartUpdate();
    }
}

void UIPlayMenuMap::OnDisable()
{
    if (_camera && Map::Instance && Map::Instance->Terrain)
    {
        _camera->SetIsActive(false);
        Map::Instance->StopUpdate();
        Map::Instance->Terrain->SetSplatmap(TestTerrain::SplatmapType::Game);
    }
}

void UIPlayMenuMap::OnUpdate()
{
    if (!_camera || !Map::Instance || !Map::Instance->Terrain)
    {
        return;
    }

    Vector3 _currentPosition = _camera->GetParent()->GetPosition();
    float _currentPitch = _camera->GetParent()->GetOrientation().GetEuler().X;
    float _currentDistance = _camera->GetLocalPosition().Z;

    if (Input::GetMouseButton(MouseButton::Right))
    {
        float move = Math::Remap(_currentDistance, -Map::Instance->MinZoom, -Map::Instance->MaxZoom, _moveMin, _moveMax);
        float forward = Input::GetAxis(TEXT("Mouse X")) * _mouseSensitivity * move * Time::GetDeltaTime(); // H
        float side = Input::GetAxis(TEXT("Mouse Y")) * _mouseSensitivity * move * Time::GetDeltaTime(); // V

        if (_camera)
        {
            _targetPosition = _camera->GetParent()->GetPosition() + Vector3(-forward, 0.0f, side);
        }
    }

    float scroll = Input::GetMouseScrollDelta();

    if (scroll != 0.0f)
    {
        _zoomStep = Math::Remap(_currentDistance, -Map::Instance->MinZoom, -Map::Instance->MaxZoom, _zoomStepMin, _zoomStepMax);
        _targetPitch = Math::Remap(_currentDistance, -Map::Instance->MinZoom, -Map::Instance->MaxZoom, _pitchMin, _pitchMax);
        if (scroll > 0.0f)
        {
            _targetDistance += _zoomStep;
        }
        else
        {
            _targetDistance -= _zoomStep;
        }
        _targetDistance = Math::Clamp(_targetDistance, -Map::Instance->MaxZoom, -Map::Instance->MinZoom);
    }

    // Position
    _camera->GetParent()->SetPosition(Vector3::Lerp(_currentPosition, _targetPosition, 0.33f));
    // Orientation
    _camera->GetParent()->SetOrientation(Quaternion::Euler(Math::Lerp(_currentPitch, _targetPitch, 0.33f), 0.0f, 0.0f));
    // Distance
    _camera->SetLocalPosition(Vector3(0.0f, 0.0f, Math::Lerp(_currentDistance, _targetDistance, 0.33f)));

    if (Map::Instance)
    {
        Map::Instance->Update();
    }
}

void UIPlayMenuMap::ProcessEvent(Rml::Event& event)
{
    using namespace Rml;

    switch (event.GetId())
    {
    case EventId::Click:
    {
        auto el = event.GetTargetElement();
        if (el->GetId() == "setGame")
        {
            Map::Instance->Terrain->SetSplatmap(TestTerrain::SplatmapType::Game);
        }
        else if (el->GetId() == "setResources")
        {
            Map::Instance->Terrain->SetSplatmap(TestTerrain::SplatmapType::MapResources);
        }
    }
    break;

    default:
        break;
    }
}