#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/Components/IComponent.h>

API_CLASS() class GAME_API RobotAI : public IComponent, public INetworkObject
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(RobotAI);

public:

    API_FIELD(NetworkReplicated) uint32 State1 = 0;
    API_FIELD(NetworkReplicated) uint32 State2 = 0;
    API_FIELD(NetworkReplicated) uint32 State3 = 0;

    void OnNetworkSpawn() override;
};
