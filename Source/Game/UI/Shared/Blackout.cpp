#include "Blackout.h"

Blackout::Blackout(Rml::ElementDocument* document, Function<void()> callback)
    : _document(document),
    _callback(callback)
{
    _blackout = document->GetElementById("blackout");
}

void Blackout::StartFade(float duration)
{
    if (_blackingOut)
    {
        return;
    }

    _blackout->Animate("opacity", Rml::Property(1.0f, Rml::Unit::NUMBER), duration, {}, false, false, 0.0f);
}

void Blackout::FadeIn()
{
    _blackout->SetProperty("opacity", "1");
}

void Blackout::ClearFade(float duration)
{
    _blackout->Animate("opacity", Rml::Property(0.0f, Rml::Unit::NUMBER), duration, {}, false, false, 0.0f);
}

void Blackout::FadeOut()
{
    _blackout->SetProperty("opacity", "0");
}

void Blackout::ProcessEvent(Rml::Event& event)
{
    using namespace Rml;

    switch (event.GetId())
    {
    case EventId::Animationend:
    {
        auto el = event.GetTargetElement();
        if (el->GetId() == "blackout" && el->GetProperty("opacity")->Get<float>() >= 0.98f)
        {
            _blackingOut = false;
            _callback();
        }
    }
    break;

    default:
        break;
    }
}
