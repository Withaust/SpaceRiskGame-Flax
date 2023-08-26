#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Input/Input.h>
#include <Engine/Engine/Screen.h>
#include <Engine/Scripting/Enums.h>

#include <Game/UI/IUIUnit.h>
#include <Game/Shared/Components/IInteractable.h>
#include <Game/System/Game/Player/PlayerManager.h>
#include <Game/Dynamic/Pawns/Humanlike/Player/PlayerInteractor.h>

API_CLASS() class GAME_API UIInteractor : public IUIUnit
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(UIInteractor);

private:

    bool _isVisible = false;
    Rml::Element* _menu = nullptr;
    Rml::ElementPtr _entry;
    int _counter = 0;
    Array<InteractableOption> _options;

public:

    void OnInitialize() override;
    void OnDeinitialize() override;
    void OnUpdate() override;

    void OnShowMenu(IInteractable* interactible);
    void OnHideMenu(IInteractable* interactible);
};
