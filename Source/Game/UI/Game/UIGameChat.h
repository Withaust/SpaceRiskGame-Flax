﻿#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Input/Input.h>
#include <Engine/Engine/Screen.h>

#include <Game/UI/IUIUnit.h>
#include <Game/System/Game/Chat.h>
#include <Game/System/Game/Player/PlayerManager.h>
#include <Game/Dynamic/Pawns/Humanlike/Player/PlayerMovement.h>

API_CLASS() class GAME_API UIGameChat : public IUIUnit
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(UIGameChat);
private:

    bool _isFocused = false;
    Rml::Element* _window = nullptr;
    Rml::Element* _textBox = nullptr;
    Rml::Element* _root = nullptr;
    Rml::ElementPtr _entry;
    int _counter = 0;

public:

    bool IsFocused();

    void OnInitialize() override;
    void OnDeinitialize() override;
    void OnUpdate() override;

    void OnChatMessage(uint32 Sender, String Message);
    void Focus();
    void Unfocus(bool AnimateWindow = false);
};
