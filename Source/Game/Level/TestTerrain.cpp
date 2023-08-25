#include "TestTerrain.h"

TestTerrain::TestTerrain(const SpawnParams& params)
    : Script(params)
{
}

void ColorCompare(const byte& Color, byte& CurrentColor, TestTerrain::TerrainType& currentType, const Dictionary<uint8, TestTerrain::TerrainType>& terrainTypes, byte index)
{
    if (Color > CurrentColor)
    {
        CurrentColor = Color;
        if (terrainTypes.ContainsKey(index))
        {
            currentType = terrainTypes[index];
        }
        else
        {
            currentType = TestTerrain::TerrainType::Grass;
        }
    }
}

void TestTerrain::OnEnable()
{
    _terrain = Cast<Terrain>(GetActor());

    for (int i = 0; i < _terrain->GetPatchesCount(); ++i)
    {
        TerrainPatch* patch = _terrain->GetPatch(i);

        Array<TerrainType> type;

        const int32 heightMapSize = _terrain->GetChunkSize() * TerrainPatch::CHUNKS_COUNT_EDGE + 1;
        const int32 heightMapLength = heightMapSize * heightMapSize;

        type.Resize(heightMapLength);

        Color32* layer1 = patch->GetSplatMapData(0);
        Color32* layer2 = patch->GetSplatMapData(1);
        byte currentColor = 0;
        TerrainType currentType = TerrainType::Grass;

        for (int i = 0; i < type.Count(); ++i)
        {
            currentColor = 0;
            currentType = TerrainType::Grass;

            if (layer1)
            {
                const auto& color = layer1[i];
                ColorCompare(color.R, currentColor, currentType, TerrainTypes, 0);
                ColorCompare(color.G, currentColor, currentType, TerrainTypes, 1);
                ColorCompare(color.B, currentColor, currentType, TerrainTypes, 2);
                ColorCompare(color.A, currentColor, currentType, TerrainTypes, 3);
            }
            if (layer2)
            {
                const auto& color = layer2[i];
                ColorCompare(color.R, currentColor, currentType, TerrainTypes, 4);
                ColorCompare(color.G, currentColor, currentType, TerrainTypes, 5);
                ColorCompare(color.B, currentColor, currentType, TerrainTypes, 6);
                ColorCompare(color.A, currentColor, currentType, TerrainTypes, 7);
            }

            type[i] = currentType;
        }

        _cachedChunks[Int2(patch->GetX(), patch->GetZ())] = type;

        patch->ClearSplatMapCache();
    }
}

TestTerrain::TerrainType TestTerrain::GetTerrainType(const Vector3& position, const Vector3& direction, float distance)
{
    TerrainType result = TerrainType::Grass;

    int32 splatmapSize = _terrain->GetChunkSize() * TerrainPatch::CHUNKS_COUNT_EDGE + 1;
    float splatmapSizeWorld = _terrain->GetPatch(0)->GetBounds().GetSize().X;

    Transform local;
    _terrain->GetTransform().WorldToLocal(Transform(position, {}), local);
    Vector3 localPosition = local.Translation;

    Int2 patchCoord((int32)(localPosition.X / splatmapSizeWorld), (int32)(localPosition.Z / splatmapSizeWorld));

    TerrainPatch* patch = _terrain->GetPatch(patchCoord);

    if (!patch)
    {
        return result;
    }

    if (!_cachedChunks.ContainsKey(patchCoord))
    {
        return result;
    }

    BoundingBox bb = _terrain->GetBox();
    Vector3 totalTerrainSize = bb.GetSize();
    Vector3 terrainOffset = bb.Minimum;

    int32 x = (int32)(localPosition.X / (totalTerrainSize.X / splatmapSize));
    int32 z = (int32)(localPosition.Z / (totalTerrainSize.Z / splatmapSize));

    int32 index = z * splatmapSize + x;
    const auto& chunk = _cachedChunks[patchCoord];

    if (index < 0 || index >= chunk.Count())
    {
        return result;
    }

    return chunk[index];
}

void TestTerrain::OnDisable()
{
    // Here you can add code that needs to be called when script is disabled (eg. unregister from events)
}
