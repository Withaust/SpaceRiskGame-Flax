#include "UISystemMenu.h"

UISystemMenu::UISystemMenu(const SpawnParams& params)
    : IUIState(params)
{
}

String UISystemMenu::GetBackState()
{
    return TEXT("Game.UIGame");
}
