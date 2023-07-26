#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/Shared/Defines.h>
#include <Game/UI/IUIUnit.h>

#include <RmlUi/RmlUiDocument.h>

API_CLASS() class GAME_API IUIState : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(IUIState);
private:
    Array<IUIUnit*> _units;

public:
    API_FUNCTION() virtual void OnInitialize();
    API_FUNCTION() virtual void OnDeinitialize();
    API_FUNCTION() virtual String GetBackState() { return TEXT("!"); };
    API_FUNCTION() RmlUiDocument* GetDocument();
};
