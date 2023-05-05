#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/Shared/Utils/Defines.h>
#include <Game/Shared/INetworked.h>
#include <Game/System/Core/Steam.h>
#include <Game/System/Game/PlayerManager.h>

API_CLASS() class GAME_API PlayerInfo : public INetworked
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerInfo);
    friend class PlayerManager;
private:
    bool clientRegistered = false;
public:

    API_FIELD(NetworkReplicated) int32 Id;
    API_FIELD(NetworkReplicated) String Name;
    API_FIELD(NetworkReplicated) int Level;
    API_FIELD(NetworkReplicated) int Skill1;
    API_FIELD(NetworkReplicated) int Skill2;
    API_FIELD(NetworkReplicated) int TemplateId;

    void OnSpawn() override;
    void OnDespawn() override;
    void OnSync() override;
};
