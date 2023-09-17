#pragma once

#include <Engine/Scripting/Script.h>

#include <RmlUi/RmlUiDocument.h>
#include <RmlUi/IncludeRmlUiCore.h>

#include <Game/UI/Shared/Blackout.h>

class Blackout : public Rml::EventListener
{
private:
    Rml::ElementDocument* _document = nullptr;
    Rml::Element* _blackout = nullptr;
    Function<void()> _callback;
    bool _blackingOut = false;

public:
    Blackout(Rml::ElementDocument* document, Function<void()> callback);
    // Enables fade with a delay
    void StartFade(float duration);
    // Enables fade instantly
    void FadeIn();

    // Removes fade with a delay
    void ClearFade(float duration);
    // Removes fade instantly
    void FadeOut();

    void ProcessEvent(Rml::Event& event) override;
};
