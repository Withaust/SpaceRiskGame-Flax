#include "IComponent.h"

IComponent::IComponent(const SpawnParams& params)
    : ISpawnSync(params)
{
}

Entity* IComponent::GetEntity()
{
    if (!_entity)
    {
        _entity = Cast<Entity>(GetParent());
    }
    return _entity;
}
