#include "MapLabel.h"

MapLabel::MapLabel(const SpawnParams& params)
    : Script(params)
{
}

void MapLabel::OnStart()
{
    if (TextRender* text = Cast<TextRender>(GetActor()))
    {
        Map::Instance->AddLabel(text);
    }
}

void MapLabel::OnDestroy()
{
    if (TextRender* text = Cast<TextRender>(GetActor()))
    {
        Map::Instance->RemoveLabel(text);
    }
}