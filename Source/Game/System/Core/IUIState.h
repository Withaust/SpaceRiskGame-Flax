#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/Shared/Utils/Defines.h>

API_CLASS() class GAME_API IUIState : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(IUIState);
public:

    API_FUNCTION() virtual void OnInitialize() {};
    API_FUNCTION() virtual void OnDeinitialize() {};
    API_FUNCTION() virtual String GetBackState() { return TEXT("!"); };
};
