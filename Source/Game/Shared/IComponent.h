#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actor.h>
#include <Game/Shared/Entity.h>

API_CLASS() class GAME_API IComponent : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(IComponent);
    friend class Entity;
private:

    Entity* _entity = nullptr;
    
public:
    Entity* GetEntity();
};
