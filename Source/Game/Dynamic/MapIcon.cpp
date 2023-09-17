#include "MapIcon.h"

MapIcon::MapIcon(const SpawnParams& params)
    : Script(params)
{
}

void MapIcon::OnEnable()
{
    if (SpriteRender* icon = Cast<SpriteRender>(GetActor()))
    {
        Map::Instance->AddIcon(icon);
    }
}

void MapIcon::OnDisable()
{
    if (SpriteRender* icon = Cast<SpriteRender>(GetActor()))
    {
        Map::Instance->RemoveIcon(icon);
    }
}