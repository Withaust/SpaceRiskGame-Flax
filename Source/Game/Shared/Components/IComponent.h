#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actor.h>
#include <Engine/Networking/NetworkRpc.h>

#include <Game/Shared/INetworkedObject.h>
#include <Game/Shared/Entity.h>

API_CLASS() class GAME_API IComponent : public INetworkedObject
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(IComponent);
    friend class Entity;
    friend class Networking;

private:
    Entity* _entity = nullptr;
    
public:
    API_FUNCTION() Entity* GetEntity();
};
