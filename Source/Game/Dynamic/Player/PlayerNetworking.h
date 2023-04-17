#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actor.h>
#include <Engine/Networking/NetworkManager.h>
#include <Engine/Networking/NetworkReplicator.h>
#include <Engine/Networking/INetworkObject.h>
#include <Engine/Level/Actors/Camera.h>

#include <Game/Shared/Utils/Defines.h>

API_CLASS() class GAME_API PlayerNetworking : public Script, public INetworkObject
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerNetworking);
public:

    API_FIELD() ScriptingObjectReference<Camera> Camera;

    void ClaimAuthority();
    void OnNetworkSpawn() override;

    void OnNetworkDespawn() override {};
    void OnNetworkSerialize() override {};
    void OnNetworkDeserialize() override {};
};
