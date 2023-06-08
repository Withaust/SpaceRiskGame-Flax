#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/Shared/Defines.h>
#include <Game/System/Core/IUIState.h>

API_CLASS() class GAME_API UI : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(UI);
private:

    ScriptingObjectReference<IUIState> _currentState;
    Dictionary<String, IUIState*> _states;

    void FindStates(Actor* target, Array<IUIState*>& result);
    void FindScripts(Actor* target, Array<Script*>& result);

public:

    void OnInitialize() override;
    void OnDeinitialize() override;

    API_FUNCTION() void GoForward(String state);
    API_FUNCTION() void GoBack();
};
