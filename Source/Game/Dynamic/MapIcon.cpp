#include "MapIcon.h"

MapIcon::MapIcon(const SpawnParams& params)
    : IComponent(params)
{
    _tickUpdate = true;
}

void MapIcon::OnStart()
{
    if (!Static)
    {
        _update = 4.0f;
    }
    if (Sprite)
    {
        Map::Instance->AddIcon(Sprite);
        Sprite->SetParent(Sprite->GetScene());
    }
}

void MapIcon::OnUpdate()
{
    if (Static)
    {
        return;
    }

    Sprite->SetPosition(Root->GetPosition());
}

void MapIcon::OnDestroy()
{
    if (Sprite)
    {
        Map::Instance->RemoveIcon(Sprite);
    }
}