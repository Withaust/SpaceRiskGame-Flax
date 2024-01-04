#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/Components/IComponent.h>

API_CLASS() class GAME_API RobotSquad : public IComponent, public INetworkObject
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(RobotSquad);

public:

    API_FIELD(NetworkReplicated) Guid SquadId;

    void OnNetworkSpawn() override;
};
