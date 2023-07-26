#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Input/Input.h>

#include <Game/UI/IUIUnit.h>
#include <Game/System/Game/Chat.h>
#include <Game/System/Game/Player/PlayerManager.h>

API_CLASS() class GAME_API UIChat : public IUIUnit
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(UIChat);
private:

    bool _isFocused = false;
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
    void Unfocus();
};
