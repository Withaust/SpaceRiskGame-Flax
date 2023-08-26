#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Physics/Physics.h>
#include <Engine/Physics/Colliders/Collider.h>
#include <Engine/Core/Types/LayersMask.h>
#include <Engine/Level/Actor.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/Components/IInteractable.h>

API_CLASS() class GAME_API PlayerInteractor : public IComponent
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerInteractor);

private:

    SleepBlock _sleep;
    ScriptingObjectReference<IInteractable> _target;
    bool _showing = false;

    void Show();
    void Hide();

public:

    static Delegate<IInteractable*> OnShowMenu;
    static Delegate<IInteractable*> OnHideMenu;

    API_FIELD() ScriptingObjectReference<Actor> RaycastOrigin;
    API_FIELD() float RaycastDistance = 150.0f;
    API_FIELD() LayersMask RaycastMask;

    void ExecuteOption(InteractableOption& option);

    void OnEnable() override;
    void OnUpdate() override;

};
