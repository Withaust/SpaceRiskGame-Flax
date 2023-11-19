#include "Map.h"

UIMPL_SINGLETON(Map)

Map::Map(const SpawnParams& params)
    : ISystem(params)
{
}

const HashSet<TextRender*> Map::GetLabels()
{
    return _labels;
}

const HashSet<SpriteRender*> Map::GetIcons()
{
    return _icons;
}

void Map::OnDeinitialize()
{
    UDEINIT_SINGLETON();
}

void Map::StartUpdate()
{
    for (const auto& target : _labels)
    {
        target.Item->SetIsActive(true);
    }

    for (const auto& target : _icons)
    {
        target.Item->SetIsActive(true);
    }
}

void Map::Update()
{
    if (!Camera)
    {
        return;
    }

    for (const auto& target : _labels)
    {
        const auto label = target.Item;
        float distance = Vector3::Distance(Camera->GetPosition(), label->GetPosition());
        float scale = Math::Remap(distance, MinZoom, MaxZoom, MinLabel, MaxLabel);
        label->SetScale(Vector3(scale, scale, scale));
    }

    for (const auto& target : _icons)
    {
        const auto icon = target.Item;
        float distance = Vector3::Distance(Camera->GetPosition(), icon->GetPosition());
        float scale = Math::Remap(distance, MinZoom, MaxZoom, MinIcon, MaxIcon);
        icon->SetSize(Float2(scale, scale));
    }
}

void Map::StopUpdate()
{
    for (const auto& target : _labels)
    {
        target.Item->SetIsActive(false);
    }

    for (const auto& target : _icons)
    {
        target.Item->SetIsActive(false);
    }
}

void Map::AddLabel(TextRender* text)
{
    _labels.Add(text);
    text->SetIsActive(false);
}

void Map::RemoveLabel(TextRender* text)
{
    _labels.Remove(text);
}

void Map::AddIcon(SpriteRender* icon)
{
    _icons.Add(icon);
    icon->SetIsActive(false);
}

void Map::RemoveIcon(SpriteRender* icon)
{
    _icons.Remove(icon);
}
