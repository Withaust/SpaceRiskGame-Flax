#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actor.h>
#include <Engine/Debug/DebugDraw.h>
#include <Engine/Renderer/DrawCall.h>
#include <Engine/Content/Content.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/Components/IComponent.h>
#include <Game/Level/Player/PlayerSpawn.h>
#include <Game/System/Game/Player/PlayerRespawns.h>

API_CLASS() class GAME_API PlayerSpawnGroup : public IComponent
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerSpawnGroup);
    friend class PlayerRespawn;

public:

    API_FIELD() Array<ScriptingObjectReference<PlayerSpawn>> Spawns;

    void OnEnable() override;
    void OnDisable() override;
#if USE_EDITOR
    void OnDebugDrawSelected() override;
#endif
};
