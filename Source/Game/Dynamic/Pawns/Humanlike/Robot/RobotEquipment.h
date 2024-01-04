#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Core/IBinarySerializable.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/Components/IComponent.h>

API_STRUCT(NoDefault) struct EquipmentData
{
    DECLARE_SCRIPTING_TYPE_STRUCTURE(EquipmentData);
    API_FIELD() Guid Id;
    API_FIELD() bool Amount;
};

template<>
struct TIsPODType<EquipmentData>
{
    enum { Value = true };
};

#include "Engine/Serialization/Serialization.h"
namespace Serialization
{
    inline bool ShouldSerialize(const EquipmentData& v, const void* otherObj) { return true; }
    inline void Serialize(ISerializable::SerializeStream& stream, const EquipmentData& v, const void* otherObj)
    {
        USERIALIZE_MEMBER(Amount);
        USERIALIZE_MEMBER(Id);
    }
    inline void Deserialize(ISerializable::DeserializeStream& stream, EquipmentData& v, ISerializeModifier* modifier)
    {
        UDESERIALIZE_MEMBER(Amount);
        UDESERIALIZE_MEMBER(Id);
    }
}

API_CLASS() class GAME_API RobotEquipment : public IComponent, public INetworkObject
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(RobotEquipment);
public:
    API_FIELD(NetworkReplicated) Array<EquipmentData> Equipment;

    void OnNetworkSpawn() override;
};
