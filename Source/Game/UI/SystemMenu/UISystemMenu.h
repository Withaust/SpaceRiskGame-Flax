#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Engine/Screen.h>
#include <Engine/Input/Input.h>

#include <Game/UI/IUIState.h>

API_CLASS() class GAME_API UISystemMenu : public IUIState
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(UISystemMenu);

public:

    String GetBackState() override;
};
