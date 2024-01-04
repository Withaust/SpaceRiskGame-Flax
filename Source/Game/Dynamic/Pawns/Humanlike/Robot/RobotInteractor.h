#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/Components/IComponent.h>

API_CLASS() class GAME_API RobotInteractor : public IComponent, public INetworkObject
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(RobotInteractor);

public:

    API_FIELD(NetworkReplicated) Guid InteractionTarget;
    API_FIELD(NetworkReplicated) int InteractionType = 0;

    void OnNetworkSpawn() override;
};
