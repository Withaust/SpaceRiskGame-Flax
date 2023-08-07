#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/NetworkRpc.h>
#include <Engine/Networking/NetworkManager.h>

#include <Game/Shared/Defines.h>

API_CLASS() class GAME_API KillFeed : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(KillFeed);

public:
    API_FIELD() static KillFeed* Instance;
    API_EVENT() Delegate<Entity*, Entity*> OnKill;
};
