#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actor.h>
#include <Engine/Networking/NetworkRpc.h>

#include <Game/Shared/Entity.h>
#include <Game/System/Core/Networking/Networking.h>

API_CLASS() class GAME_API IComponent : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(IComponent);
    friend class Entity;
    friend class Networking;

private:

    Entity* _entity = nullptr;
    
public:
    API_FIELD() bool SyncOnlySpawn = false;
    Entity* GetEntity();
    API_FUNCTION(NetworkRpc = "Client, Reliable") void SendData(NetworkRpcParams info, Array<byte> data);
};
