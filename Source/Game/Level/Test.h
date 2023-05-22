#pragma once

#include <Engine/Scripting/Script.h>
#include <Game/Shared/Utils/Defines.h>

API_CLASS(Attributes = "ActorContextMenu(\"New/Other/Test\"), ActorToolbox(\"Other\")")
class GAME_API Test : public Actor
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Test);

public:

    API_FIELD() Array<Actor*> Spawns;

    void OnEnable() override;
    void OnDisable() override;
};
