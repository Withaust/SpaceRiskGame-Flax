#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Physics/Physics.h>
#include <Engine/Core/Types/LayersMask.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Scripting/Enums.h>

#include <Game/Shared/Defines.h>
#include <Game/Level/TestTerrain.h>

API_CLASS() class GAME_API Footsteps : public IComponent
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Footsteps);

private:

    SleepBlock _sleep;

public:

    API_FIELD() ScriptingObjectReference<Actor> RaycastOrigin;
    API_FIELD() LayersMask RaycastMask;

    void OnEnable() override;
    void OnUpdate() override;
};
