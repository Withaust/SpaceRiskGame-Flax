#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/Shared/Utils/Defines.h>

API_STRUCT() struct GAME_API CustomStruct
{
    DECLARE_SCRIPTING_TYPE_STRUCTURE(CustomStruct);
    API_FIELD(NetworkReplicated) int32 MyVar = 0.0f;
    API_FIELD(NetworkReplicated) String TestString;
};

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
