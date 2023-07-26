#include "UIGame.h"

UIGame::UIGame(const SpawnParams& params)
    : IUIState(params)
{
    _tickUpdate = true;
}

void UIGame::OnInitialize()
{
    Screen::SetCursorLock(CursorLockMode::Locked);
    Screen::SetCursorVisible(false);
    IUIState::OnInitialize();
}

void UIGame::OnDeinitialize()
{
    Screen::SetCursorLock(CursorLockMode::None);
    Screen::SetCursorVisible(true);
    IUIState::OnDeinitialize();
}

void UIGame::OnUpdate()
{
    if (Input::GetKeyUp(KeyboardKeys::T) && !Chat->IsFocused())
    {
        Chat->Focus();
    }
}

String UIGame::GetBackState()
{
    return TEXT("None");
}
