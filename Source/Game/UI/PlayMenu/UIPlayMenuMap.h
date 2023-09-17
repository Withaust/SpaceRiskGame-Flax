#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Input/Input.h>
#include <Engine/Streaming/StreamingSettings.h>

#include <ThirdParty/RmlUi/Core/ElementUtilities.h>
#include <ThirdParty/RmlUi/Core/ComputedValues.h>

#include <Game/UI/IUIUnit.h>
#include <Game/UI/UI.h>
#include <Game/System/Game/Map.h>


API_CLASS() class GAME_API UIPlayMenuMap : public IUIUnit
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(UIPlayMenuMap);
private:

    ScriptingObjectReference<Camera> _camera;

    float _yaw = 0.0f;
    float _pitch = 0.0f;
    float _mouseSensitivity = 100.0f;
    float _pitchMin = 45.0f;
    float _pitchMax = 80.0f;
    float _distance = 0.0f;

    float _zoomStepMin = 1000.0f;
    float _zoomStep = 500.0f;
    float _zoomStepMax = 6000.0f;

public:

    void OnInitialize() override;
    void OnDeinitialize() override;
    void OnEnable() override;
    void OnDisable() override;
    void OnUpdate() override;
};
