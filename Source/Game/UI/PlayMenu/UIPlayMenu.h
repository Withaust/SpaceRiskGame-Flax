#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Engine/Screen.h>
#include <Engine/Input/Input.h>
#include <Engine/Streaming/StreamingSettings.h>

#include <Game/UI/IUIState.h>
#include <Game/UI/UI.h>
#include <Game/UI/Shared/Blackout.h>

API_CLASS() class GAME_API UIPlayMenu : public IUIState
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(UIPlayMenu);

private:

    Rml::ElementDocument* _document = nullptr;
    Blackout* _blackout = nullptr;

public:

    void StartBlackout();
    void OnEndBlackout();
    void OnInitialize() override;
    void OnDeinitialize() override;
    void OnEnable() override;
    void OnDisable() override;

    void OnUpdate() override;
    String GetBackState() override;
};
