#pragma once

#include <Engine/Scripting/Script.h>

#include <RmlUi/RmlUiDocument.h>
#include <RmlUi/IncludeRmlUiCore.h>

API_CLASS() class GAME_API IUIUnit : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(IUIUnit);
public:
    API_FUNCTION() virtual void OnInitialize() {};
    API_FUNCTION() virtual void OnDeinitialize() {};
    API_FUNCTION() RmlUiDocument* GetDocument();
};
