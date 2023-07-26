#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Engine/Screen.h>
#include <Engine/Input/Input.h>

#include <Game/UI/IUIState.h>
#include <Game/UI/UIChat.h>

API_CLASS() class GAME_API UIGame : public IUIState
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(UIGame);

public:

    API_FIELD() ScriptingObjectReference<UIChat> Chat;

    void OnInitialize() override;
    void OnDeinitialize() override;
    void OnUpdate() override;
    String GetBackState() override;
};
