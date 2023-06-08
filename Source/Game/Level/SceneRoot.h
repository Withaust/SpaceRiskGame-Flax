#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Content/Content.h>
#include <Engine/Content/AssetInfo.h>
#include <Engine/Core/Config/GameSettings.h>

#include <Game/Shared/Defines.h>

API_CLASS() class GAME_API SceneRoot : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(SceneRoot);

    API_FIELD() static String EditorScene;

    void OnAwake() override;
};
