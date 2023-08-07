#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/INetworkObject.h>
#include <Engine/Level/Actor.h>
#include <Engine/Networking/NetworkManager.h>
#include <Engine/Networking/NetworkReplicator.h>
#include <Engine/Level/Actors/Camera.h>
#include <Engine/Content/Assets/Material.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/Components/IComponent.h>
#include <Game/Shared/Layers.h>
#include <Game/System/Game/Player/PlayerManager.h>
#include <Game/System/Game/PostFx.h>
#include <Game/Dynamic/Pawns/PawnInfo.h>

API_CLASS() class GAME_API PlayerNetworking : public IComponent, public INetworkObject
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerNetworking);

public:

    API_FIELD() ScriptingObjectReference<Camera> Camera;
    API_FIELD() ScriptingObjectReference<Actor> Direction;

    void OnNetworkSpawn() override;
    void OnNetworkDespawn() override;
};
