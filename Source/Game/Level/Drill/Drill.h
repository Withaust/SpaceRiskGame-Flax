#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actor.h>
#include <Engine/UI/TextRender.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/Components/IComponent.h>

API_CLASS(Attributes = "ContentContextMenu(\"New/Data/DrillData\")")
class GAME_API DrillData : public ISerializable
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(DrillData);

    API_FIELD() float MaxFuel = 0.0f;
    API_FIELD() float GenerateSpeed = 0.0f;
    API_FIELD() String Name;
};

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

    API_PROPERTY(NetworkReplicated) float GetFuelLocal() const { UNETPROP_GETLOCAL(_fuel); }
    API_PROPERTY(NetworkReplicated) void SetFuelLocal(float value);
    API_FUNCTION(NetworkRpc = "Server, ReliableOrdered") void SetFuelRemote(float value)
    { UNETPROP_SETREMOTE(Drill, Fuel); }
    API_FUNCTION(NetworkRpc = "Client, ReliableOrdered") void SetFuelSync(float value, NetworkRpcParams p)
    { UNETPROP_SETSYNC(Drill, Fuel); }

    API_FIELD(NetworkReplicated, Attributes = "AssetReference(typeof(DrillData))") AssetReference<JsonAsset> Data;
    UDECLARE_DATA(DrillData, Data);
    API_FUNCTION(NetworkRpc = "Server, ReliableOrdered") void SetDataRemote(const AssetReference<JsonAsset>& value)
    { UNETPROP_SETREMOTE(Drill, Data); }
    API_FUNCTION(NetworkRpc = "Client, ReliableOrdered") void SetDataSync(const AssetReference<JsonAsset>& value, NetworkRpcParams p)
    { UNETPROP_SETSYNC_DATA(Drill, Data); }

    API_FIELD() ScriptingObjectReference<Actor> Start;
    API_FIELD() ScriptingObjectReference<Actor> End;
    API_FIELD() ScriptingObjectReference<TextRender> Text;
    API_FIELD() ScriptingObjectReference<Actor> Head;

    void OnEnable() override;
    void OnUpdate() override;
};
