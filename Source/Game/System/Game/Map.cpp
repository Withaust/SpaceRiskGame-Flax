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

void Map::UpdateSizes()
{
    if (!Camera)
    {
        return;
    }

    for (const auto& target : _labels)
    {
        const auto label = target.Item;
        label->LookAt(Camera->GetPosition(), Vector3::Up);
        Vector3 orient = label->GetOrientation().GetEuler();
        orient.X = -90.0f;
        orient.Z = 0.0f;
        label->SetOrientation(Quaternion::Euler(orient));
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

void Map::AddLabel(TextRender* text)
{
    _labels.Add(text);
}

void Map::RemoveLabel(TextRender* text)
{
    _labels.Remove(text);
}

void Map::AddIcon(SpriteRender* icon)
{
    _icons.Add(icon);
}

void Map::RemoveIcon(SpriteRender* icon)
{
    _icons.Remove(icon);
}
