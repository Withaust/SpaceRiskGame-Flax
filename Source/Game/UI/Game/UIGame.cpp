#include "UIGame.h"

UIGame::UIGame(const SpawnParams& params)
    : IUIState(params)
{
    _tickUpdate = true;
}

void UIGame::StartBlackout()
{
    _blackout->StartFade(0.5f);
}

void UIGame::OnEndBlackout()
{
    for (auto& group : StreamingSettings::Get()->TextureGroups)
    {
        if (group.Name == TEXT("Terrain"))
        {
            group.MaxAnisotropy = 1;
            group.MipLevelsMax = 1;
            StreamingSettings::Get()->Apply();
            break;
        }
    }
    UI::Instance->GoForward(TEXT("Game.UIPlayMenu"));
}

void UIGame::OnInitialize()
{
    Function<void()> callback;
    callback.Bind<UIGame, &UIGame::OnEndBlackout>(this);
    _document = GetDocument()->GetDocument();
    _blackout = New<Blackout>(_document, callback);
    _document->AddEventListener(Rml::EventId::Animationend, _blackout);
    _blackout->FadeIn();
    IUIState::OnInitialize();
}

void UIGame::OnDeinitialize()
{
    _document->RemoveEventListener(Rml::EventId::Animationend, _blackout);
    Delete(_blackout);

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
    IUIState::OnDeinitialize();
}

void UIGame::OnEnable()
{
    if (_blackout)
    {
        _blackout->ClearFade(0.5f);
    }

    Screen::SetCursorLock(CursorLockMode::Locked);
    Screen::SetCursorVisible(false);

    if (PlayerManager::Instance)
    {
        if (auto player = PlayerManager::Instance->GetOurPlayer())
        {
            player->GetComponent<PlayerMovement>()->CanMove = true;
            player->GetComponent<PlayerNetworking>()->Camera->SetIsActive(true);
        }
    }
}

void UIGame::OnDisable()
{
    Screen::SetCursorLock(CursorLockMode::None);
    Screen::SetCursorVisible(true);

    if (PlayerManager::Instance)
    {
        if (auto player = PlayerManager::Instance->GetOurPlayer())
        {
            player->GetComponent<PlayerMovement>()->CanMove = false;
            player->GetComponent<PlayerNetworking>()->Camera->SetIsActive(false);
        }
    }
}

void UIGame::OnUpdate()
{
    if (Input::GetKeyUp(KeyboardKeys::Tab))
    {
        StartBlackout();
    }
}

String UIGame::GetBackState()
{
    return TEXT("Game.UISystemMenu");
}
