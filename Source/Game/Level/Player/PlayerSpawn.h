#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actor.h>
#include <Engine/Debug/DebugDraw.h>
#include <Engine/Renderer/DrawCall.h>
#include <Engine/Content/Content.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/Components/IComponent.h>
#include <Game/Dynamic/Pawns/Humanlike/Player/PlayerMovement.h>

API_CLASS() class GAME_API PlayerSpawn : public IComponent
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerSpawn);

public:
#if USE_EDITOR
    void OnDebugDraw() override;
#endif
};
