#include "MapLabel.h"

MapLabel::MapLabel(const SpawnParams& params)
    : Script(params)
{
}

void MapLabel::OnEnable()
{
    if (TextRender* text = Cast<TextRender>(GetActor()))
    {
        Map::Instance->AddLabel(text);
    }
}

void MapLabel::OnDisable()
{
    if (TextRender* text = Cast<TextRender>(GetActor()))
    {
        Map::Instance->RemoveLabel(text);
    }
}