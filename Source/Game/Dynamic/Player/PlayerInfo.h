#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/INetworkObject.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/IComponent.h>
#include <Game/System/Core/Steam.h>
#include <Game/System/Game/PlayerManager.h>

API_CLASS() class GAME_API PlayerInfo : public IComponent, public INetworkObject
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerInfo);
    friend class PlayerManager;
public:

    API_FIELD(NetworkReplicated) int32 Id = 0;
    API_FIELD(NetworkReplicated) String Name = TEXT("Name");
    API_FIELD(NetworkReplicated) int Level = 0;
    API_FIELD(NetworkReplicated) int Skill1 = 0;
    API_FIELD(NetworkReplicated) int Skill2 = 0;
    API_FIELD(NetworkReplicated) int TemplateId = 0;

    void OnNetworkSpawn() override;
    void OnNetworkDespawn() override;
    void OnNetworkSync() override;
};
