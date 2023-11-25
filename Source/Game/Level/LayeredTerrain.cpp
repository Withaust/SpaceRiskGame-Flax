#include "LayeredTerrain.h"
#include <Engine/Content/Factories/BinaryAssetFactory.h>

REGISTER_BINARY_ASSET(TerrainDataAsset, "Game.TerrainDataAsset", true);

TerrainDataAsset::TerrainDataAsset(const SpawnParams& params, const AssetInfo* info)
    : RawDataAsset(params, info)
{
}

LayeredTerrain::LayeredTerrain(const SpawnParams& params)
    : Script(params)
{
}

#if USE_EDITOR
void LayeredTerrain::ColorCompare(const byte& color, byte& bestColor, byte type, byte& bestType)
{
    if (color > bestColor)
    {
        bestColor = color;
        bestType = type;
    }
}

String LayeredTerrain::ValidateSplatType(TerrainSplatmapType type)
{
    String result = TEXT("");
    if (type == TerrainSplatmapType::Max)
    {
        result = TEXT("Cant use TerrainSplatmapType::Max");
    }
    ValidateFields(result);
    return result;
}

void LayeredTerrain::ValidateFields(String& result)
{
    for (const auto& patch : Textures)
    {
        if (!result.IsEmpty())
        {
            break;
        }
        const auto& textures = patch.Value;
        if (textures.Count() != static_cast<int>(TerrainSplatmapType::Max) * 2)
        {
            result = TEXT("Assigned texture count is wrong");
            break;
        }
        for (const auto& texture : textures)
        {
            if (!texture)
            {
                result = TEXT("Some textures were not assigned");
                break;
            }
        }
    }
    for (const auto& material : Materials)
    {
        if (!material.Value)
        {
            result = TEXT("Some materials were not assigned");
            break;
        }
    }

#define VALIDATE_TYPES(TypesField) {\
    int TypesField##Count = TypesField.Count(); \
    if (TypesField##Count < 1 || TypesField##Count > 8) \
    { result = String(TEXT(#TypesField)) + String(TEXT(" has count of types outside of 1 > X > 9 range")); } }

    VALIDATE_TYPES(GameTypes)
    VALIDATE_TYPES(MapResourcesTypes)

#undef VALIDATE_TYPES
}

void LayeredTerrain::RebuildData()
{
    String error = TEXT("");
    ValidateFields(error);
    if (!error.IsEmpty())
    {
        UCRIT_STR(false, error);
        return;
    }

    HashSet<Int2> TerrainChunks;
    HashSet<Int2> NewChunks;
    HashSet<Int2> DeletedChunks;

    // Fill HashSet<Int2> TerrainChunks
    for (int i = 0; i < Terrain->GetPatchesCount(); ++i)
    {
        TerrainPatch* patch = Terrain->GetPatch(i);
        Int2 coords(patch->GetX(), patch->GetZ());
        TerrainChunks.Add(coords);
    }

    for (const auto& Chunk : Data)
    {
        if (!TerrainChunks.Contains(Chunk.Key))
        {
            DeletedChunks.Add(Chunk.Key);
        }
    }

    for (const auto& Chunk : TerrainChunks)
    {
        if (!Data.ContainsKey(Chunk.Item))
        {
            NewChunks.Add(Chunk.Item);
        }
    }

    // Delete old chunks
    for (const auto& Chunk : DeletedChunks)
    {
        for (int i = 0; i < static_cast<int>(TerrainSplatmapType::Max) * 2; ++i)
        {
            Content::DeleteAsset(Data[Chunk.Item][i]->GetPath());
            Content::DeleteAsset(Textures[Chunk.Item][i]->GetPath());
        }
        Data.Remove(Chunk.Item);
        Textures.Remove(Chunk.Item);
    }

    String sceneName = Terrain->GetScene()->GetName();

    const int32 heightMapSize = Terrain->GetChunkSize() * TerrainPatch::CHUNKS_COUNT_EDGE + 1;
    const int32 heightMapLength = heightMapSize * heightMapSize;

    // Create new chunks
    for (const auto& Chunk : NewChunks)
    {
        TerrainPatch* patch = Terrain->GetPatch(Chunk.Item);
        for (int i = 0; i < TERRAIN_MAX_SPLATMAPS_COUNT; ++i)
        {
            if (patch->Splatmap[i])
            {
                Content::DeleteAsset(patch->Splatmap[i]->GetPath());
                patch->Splatmap[i] = nullptr;
            }
        }

        Array<AssetReference<Texture>> NewTextures;
        NewTextures.Resize(static_cast<int>(TerrainSplatmapType::Max) * 2);
        Array<AssetReference<TerrainDataAsset>> NewData;
        NewData.Resize(static_cast<int>(TerrainSplatmapType::Max));

        int textureIndex = 0;
        for (int i = 0; i < static_cast<int>(TerrainSplatmapType::Max); ++i)
        {
            String typeName = ScriptingEnum::ToString(static_cast<TerrainSplatmapType>(i));
            for (int k = 0; k < TERRAIN_MAX_SPLATMAPS_COUNT; ++k)
            {
                String targetPath = String::Format(TEXT("Content/SceneData/{0}/Terrain/Splatmaps/{1}x{2}_{3}_{4}.flax"),
                    sceneName, Chunk.Item.X, Chunk.Item.Y, typeName, k);
                Guid targetGuid = Guid::New();
                Content::CloneAssetFile(targetPath, TEXT("Content/SceneData/EmptySplatmap.flax"), targetGuid);
                NewTextures[textureIndex] = Content::Load<Texture>(targetPath);
                textureIndex++;
            }
            AssetReference<TerrainDataAsset> newData = Content::CreateVirtualAsset<TerrainDataAsset>();
            Array<byte> newDataBytes;
            newDataBytes.Resize(heightMapLength);
            newData->Data = newDataBytes;
            String targetPath = String::Format(TEXT("Content/SceneData/{0}/Terrain/Data/{1}x{2}_{3}.flax"),
                sceneName, Chunk.Item.X, Chunk.Item.Y, typeName);
            newData->Save(targetPath);
            NewData[i] = Content::Load<TerrainDataAsset>(targetPath);
        }

        Textures[Chunk.Item] = NewTextures;
        patch->Splatmap[0] = Textures[Chunk.Item][0];
        patch->Splatmap[1] = Textures[Chunk.Item][1];
        Data[Chunk.Item] = NewData;
    }

    // Rebuild chunk data
    for (int i = 0; i < Terrain->GetPatchesCount(); ++i)
    {
        TerrainPatch* patch = Terrain->GetPatch(i);
        Int2 coords(patch->GetX(), patch->GetZ());

        for (int i = 0; i < static_cast<int>(TerrainSplatmapType::Max); ++i)
        {
            auto& TargetData = Data[coords][i]->Data;

            Color32* layer1 = patch->GetSplatMapData(0);
            Color32* layer2 = patch->GetSplatMapData(1);
            
            for (int i = 0; i < TargetData.Count(); ++i)
            {
                byte bestColor = 0;
                byte bestIndex = 0;

                if (layer1)
                {
                    const auto& color = layer1[i];
                    ColorCompare(color.R, bestColor, 0, bestIndex);
                    ColorCompare(color.G, bestColor, 1, bestIndex);
                    ColorCompare(color.B, bestColor, 2, bestIndex);
                    ColorCompare(color.A, bestColor, 3, bestIndex);
                }
                if (layer2)
                {
                    const auto& color = layer2[i];
                    ColorCompare(color.R, bestColor, 4, bestIndex);
                    ColorCompare(color.G, bestColor, 5, bestIndex);
                    ColorCompare(color.B, bestColor, 6, bestIndex);
                    ColorCompare(color.A, bestColor, 7, bestIndex);
                }

                TargetData[i] = bestIndex;
            }

            Data[coords][i]->Save();
        }
    }

    Level::SaveScene(Terrain->GetScene());
}
#endif

TerrainSplatmapType LayeredTerrain::GetSplatmapType() const
{
    return _splatmapType;
}

void LayeredTerrain::SetSplatmapType(TerrainSplatmapType type)
{
#if USE_EDITOR
    String error = ValidateSplatType(type);
    if (!error.IsEmpty())
    {
        UCRIT_STR(false, error);
        return;
    }
#endif
    for (const auto& patch : Textures)
    {
        const auto& textures = patch.Value;
        int index = static_cast<int>(type);
        Terrain->GetPatch(patch.Key)->Splatmap[0] = textures[index * 2];
        Terrain->GetPatch(patch.Key)->Splatmap[1] = textures[(index * 2) + 1];
    }
    Terrain->Material = Materials[type];
    _splatmapType = type;
}


void LayeredTerrain::OnEnable()
{
    if (!Map::Instance || !Terrain)
    {
        return;
    }

    Map::Instance->Terrain = this;

#define BUILD_TYPES(TypesField, TypesName) {\
    Array<byte> types; \
    types.Resize(TypesField.Count()); \
    for (int i = 0; i < TypesField.Count(); ++i) \
    { \
        types[i] = static_cast<byte>(TypesField[i]); \
    } \
    _splatTypes[TerrainSplatmapType::TypesName] = types; }

    BUILD_TYPES(GameTypes, Game);
    BUILD_TYPES(MapResourcesTypes, MapResources);

#undef BUILD_TYPES

    SetSplatmapType(TerrainSplatmapType::Game);
}

void LayeredTerrain::OnDisable()
{
    Map::Instance->Terrain = nullptr;
}
