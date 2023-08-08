#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/INetworkObject.h>
#include <Engine/Input/Input.h>
#include <Engine/Debug/DebugDraw.h>
#include <Engine/Particles/ParticleEffect.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/Components/IComponent.h>
#include <Game/Shared/Components/ImmediateInfo.h>

API_CLASS(Attributes = "ContentContextMenu(\"New/Data/WeaponData\")")
class GAME_API WeaponData : public ISerializable
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(WeaponData);

    API_FIELD() String Name = TEXT("Weapon");
    API_FIELD() bool Automatic = true;
    API_FIELD() float Damage = 100.0f;
    API_FIELD() float FireRate = 0.333f;
    API_FIELD() float Distance = 1000.0f;
    API_FIELD() float Spread = 10.0f;
};

API_CLASS() class GAME_API Weapon : public IComponent, public INetworkObject
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Weapon);

private:

    ImmediateInfo* _info = nullptr;

    SleepBlock _batchBlock;

    float _nextShoot = 0.0f;

    bool _firedBefore = false;

public:

    API_FIELD() ScriptingObjectReference<Actor> Direction;
    API_FIELD() ScriptingObjectReference<Actor> Muzzle;
    API_FIELD() ScriptingObjectReference<ParticleEffect> BulletsEffect;

    void OnEnable() override;
    void OnUpdate() override;

    void ShootForVisual(Vector3 Spread = Vector3::Zero);
    Vector3 ShootForHurt();

    API_FIELD(NetworkReplicated, Attributes = "AssetReference(typeof(WeaponData))") AssetReference<JsonAsset> Data;
    UDECLARE_DATA(WeaponData, Data);
};
