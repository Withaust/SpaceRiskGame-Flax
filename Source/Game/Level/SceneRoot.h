#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Content/Content.h>
#include <Engine/Content/AssetInfo.h>

#include <Game/Shared/Utils/Defines.h>

API_CLASS() class GAME_API SceneRoot : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(SceneRoot);

    void OnAwake() override;
};
