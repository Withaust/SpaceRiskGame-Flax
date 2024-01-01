#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Engine/Screen.h>
#include <Engine/Input/Input.h>
#include <Game/System/Game/Player/PlayerManager.h>
#include <Game/Dynamic/Pawns/Humanlike/Player/PlayerMovement.h>

#include <Game/UI/UI.h>

API_CLASS() class GAME_API UICore : public IUIState
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(UICore);

private:

    Rml::Element* _infoText = nullptr;
    int _infoCounter = 0;
    Rml::Element* _warnText = nullptr;
    int _warnCounter = 0;
    Rml::Element* _errorText = nullptr;
    int _errorCounter = 0;

    Rml::Element* _window = nullptr;
    Rml::Element* _textBox = nullptr;
    Rml::Element* _root = nullptr;

    Rml::ElementPtr _entry;
    int _consoleCounter = 0;
    bool _consoleShown = false;
    bool _wasVisible = false;

    void PrintMessage(LogType type, const StringView& message);
    void OnInfo(const StringView& message);
    void OnWarning(const StringView& message);
    void OnError(const StringView& message);

public:

    void OnInitialize();
    void OnDeinitialize();
    void OnUpdate() override;
    String GetBackState() override;
};
