#include "RobotEquipment.h"

RobotEquipment::RobotEquipment(const SpawnParams& params)
    : IComponent(params)
{
}

void RobotEquipment::OnNetworkSpawn()
{
    UNOT_OWNED_RETURN();

    String entityPath = GetEntity()->GetNamePath();
    RandomStream rand(StringUtils::GetHashCode(entityPath.GetText(), entityPath.Length()));

    int range = rand.RandRange(1, 5);

    for (int i = 0; i < range; i++)
    {
        EquipmentData data;
        data.Amount = range;
        data.Id = Guid::New();
        Equipment.Push(data);
    }
}
