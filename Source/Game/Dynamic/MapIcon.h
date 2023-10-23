#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/UI/SpriteRender.h>

#include <Game/System/Game/Map.h>

API_CLASS() class GAME_API MapIcon : public IComponent
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(MapIcon);

private:

    SleepBlock _update;

public:

    API_FIELD() ScriptingObjectReference<SpriteRender> Sprite;
    API_FIELD() ScriptingObjectReference<Actor> Root;
    // Does not get automatically updated
    API_FIELD() bool Static = true;

    void OnStart() override;
    void OnUpdate() override;
    void OnDestroy() override;
};
