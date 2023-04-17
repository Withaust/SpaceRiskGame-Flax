#include "UIRoot.h"

UIRoot::UIRoot(const SpawnParams& params)
    : ISystem(params)
{
    _tickUpdate = true;
}

void UIRoot::OnEnable()
{
    Screen::SetCursorVisible(false);
    Screen::SetCursorLock(CursorLockMode::Locked);
}

void UIRoot::OnDisable()
{
	
}

void UIRoot::OnUpdate()
{
    if (Input::GetKeyUp(KeyboardKeys::Escape))
    {
        Screen::SetCursorVisible(true);
        Screen::SetCursorLock(CursorLockMode::None);
    }
}
