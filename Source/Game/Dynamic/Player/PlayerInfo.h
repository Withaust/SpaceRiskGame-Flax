#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/INetworkObject.h>

#include <Game/Shared/Utils/Defines.h>
#include <Game/System/Core/Steam.h>

API_CLASS() class GAME_API PlayerInfo : public Script, public INetworkObject
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerInfo);

public:

    API_FIELD(NetworkReplicated) String Name;
    API_FIELD(NetworkReplicated) int Level;
    API_FIELD(NetworkReplicated) int Skill1;
    API_FIELD(NetworkReplicated) int Skill2;
    API_FIELD(NetworkReplicated) int TemplateId;

    void OnEnable() override;
    void OnStart() override;
    void OnNetworkSpawn() override;
    void OnNetworkDespawn() override {};
    void OnNetworkSerialize() override {};
    void OnNetworkDeserialize() override {};
};
