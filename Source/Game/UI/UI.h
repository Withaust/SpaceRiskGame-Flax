#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Input/Input.h>
#include <RmlUi/RmlUiPlugin.h>
#include <RmlUi/RmlUiCanvas.h>

#include <Game/Shared/Defines.h>
#include <Game/UI/IUIState.h>
#include <Game/UI/UIViewports.h>

API_CLASS() class GAME_API UI : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(UI);
private:

    ScriptingObjectReference<IUIState> _currentState;
    Dictionary<String, IUIState*> _states;
    UIViewports* _textures = nullptr;

public:

    API_FIELD() static UI* Instance;

    UIViewports* GetTextures();

    void OnInitialize() override;
    void OnDeinitialize() override;
    void OnUpdate() override;

    API_FUNCTION() void GoForward(String state);
    API_FUNCTION() void GoBack();

    API_FUNCTION() static void RefreshCache();

};
