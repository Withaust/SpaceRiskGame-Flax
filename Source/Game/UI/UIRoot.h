#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Input/Input.h>
#include <Engine/Engine/Engine.h>
#include <Engine/Engine/Screen.h>

#include <Game/Shared/Utils/Defines.h>

API_CLASS() class GAME_API UIRoot : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(UIRoot);
    USINGLETON(UIRoot)

    void OnEnable() override;
    void OnDisable() override;
    void OnUpdate() override;
};
