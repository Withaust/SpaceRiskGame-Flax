#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actor.h>
#include <Engine/UI/TextRender.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/IComponent.h>

API_CLASS() class GAME_API Drill : public IComponent
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Drill);

private:

    bool _running = true;
    bool _startToEnd = true;
    float _fuel = 0;
    SleepBlock Generate;

public:

    API_PROPERTY(NetworkReplicated) float GetFuel() const;
    API_PROPERTY() void SetFuel(float value);
    API_FIELD() float MaxFuel = 60000;
    API_FIELD() float GenerateSpeed = 250;

    API_FIELD() ScriptingObjectReference<Actor> Start;
    API_FIELD() ScriptingObjectReference<Actor> End;
    API_FIELD() ScriptingObjectReference<TextRender> Text;
    API_FIELD() ScriptingObjectReference<Actor> Head;

    void OnEnable() override;
    void OnUpdate() override;
};
