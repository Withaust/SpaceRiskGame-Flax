#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actor.h>
#include <Engine/Debug/DebugDraw.h>
#include <Engine/Renderer/DrawCall.h>
#include <Engine/Content/Content.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/IComponent.h>
#include <Game/Level/PlayerSpawn.h>
#include <Game/System/Game/PlayerRespawns.h>

API_CLASS() class GAME_API PlayerSpawnGroup : public IComponent
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerSpawnGroup);

    friend class PlayerRespawn;

public:

    API_FIELD() Array<PlayerSpawn*> Spawns;

    void OnEnable() override;
    void OnDisable() override;
#if USE_EDITOR
    void OnDebugDrawSelected() override;
#endif
};
