#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actor.h>
#include <Engine/Networking/NetworkManager.h>
#include <Engine/Networking/NetworkReplicator.h>
#include <Engine/Level/Actors/Camera.h>

#include <Game/Shared/Utils/Defines.h>
#include <Game/Shared/INetworked.h>
#include <Game/System/Game/PlayerManager.h>

API_CLASS() class GAME_API PlayerNetworking : public INetworked
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerNetworking);

public:

    API_FIELD() ScriptingObjectReference<Camera> Camera;
    API_FIELD() ScriptingObjectReference<Actor> Direction;

    void OnSpawn() override;
    void OnDespawn() override {};
    void OnSync() override {};
};
