#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/INetworkObject.h>
#include <Engine/Input/Input.h>
#include <Engine/Debug/DebugDraw.h>
#include <Engine/Particles/ParticleEffect.h>
#include <Engine/Physics/Physics.h>
#include <Engine/Physics/Actors/PhysicsColliderActor.h>

#include <Game/Shared/Defines.h>
#include <Game/System/Core/Networking.h>
#include <Game/Shared/Components/IComponent.h>
#include <Game/Shared/Components/ImmediateInfo.h>
#include <Game/Shared/Components/IHitBox.h>
#include <Game/Shared/Components/IDamagePenetratable.h>

API_CLASS(Attributes = "ContentContextMenu(\"New/Data/WeaponData\")")
class GAME_API WeaponData : public ISerializable
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(WeaponData);

    API_FIELD() String Name = TEXT("Weapon");
    API_FIELD() bool Automatic = true;
    API_FIELD() float Damage = 100.0f;
    API_FIELD() float DamageSpread = 4.0f;
    API_FIELD() float FireRate = 0.333f;
    API_FIELD() float Distance = 8000.0f;
    API_FIELD() float Spread = 1.0f;
    API_FIELD() float Velocity = 30000.0f;
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

    Array<RayCastHit> results;
    RayCastHit visualResult;
    float ProcessHurt(const Array<RayCastHit>& result);

public:

    API_FIELD() LayersMask DamageMask;
    API_FIELD() ScriptingObjectReference<Actor> Direction;
    API_FIELD() ScriptingObjectReference<Actor> Muzzle;
    API_FIELD() ScriptingObjectReference<ParticleEffect> BulletsEffect;
    API_FIELD() float BulletMuzzleOffset = 160.0f;

    void OnEnable() override;
    void OnUpdate() override;

    void ShootForVisual(Vector3 Spread = Vector3::Zero, float Distance = 0.0f);
    Vector3 ShootForHurt();

    API_FIELD(NetworkReplicated, Attributes = "AssetReference(typeof(WeaponData))") AssetReference<JsonAsset> Data;
    UDECLARE_DATA(WeaponData, Data);
};
