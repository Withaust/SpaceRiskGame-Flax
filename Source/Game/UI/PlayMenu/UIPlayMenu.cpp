#include "UIPlayMenu.h"

UIPlayMenu::UIPlayMenu(const SpawnParams& params)
    : IUIState(params)
{
    _tickUpdate = true;
}

void UIPlayMenu::StartBlackout()
{
    _blackout->StartFade(0.5f);
    for (auto& group : StreamingSettings::Get()->TextureGroups)
    {
        if (group.Name == TEXT("Terrain"))
        {
            group.MaxAnisotropy = 16;
            group.MipLevelsMax = 14;
            StreamingSettings::Get()->Apply();
            break;
        }
    }
}

void UIPlayMenu::OnEndBlackout()
{
    UI::Instance->GoBack();
}

void UIPlayMenu::OnInitialize()
{
    Function<void()> callback;
    callback.Bind<UIPlayMenu, &UIPlayMenu::OnEndBlackout>(this);
    _document = GetDocument()->GetDocument();
    _blackout = New<Blackout>(_document, callback);
    _document->AddEventListener(Rml::EventId::Animationend, _blackout);
    _blackout->FadeIn();
    IUIState::OnInitialize();
}

void UIPlayMenu::OnDeinitialize()
{
    _document->RemoveEventListener(Rml::EventId::Animationend, _blackout);
    Delete(_blackout);
    IUIState::OnDeinitialize();
}

void UIPlayMenu::OnEnable()
{
    if (_blackout)
    {
        _blackout->ClearFade(0.5f);
    }
}

void UIPlayMenu::OnDisable()
{

}

void UIPlayMenu::OnUpdate()
{
    if (Input::GetKeyUp(KeyboardKeys::Tab) || Input::GetKeyUp(KeyboardKeys::Escape))
    {
        StartBlackout();
    }
}

String UIPlayMenu::GetBackState()
{
    return TEXT("Game.UIGame");
}
