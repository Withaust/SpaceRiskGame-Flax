#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Core/Collections/Dictionary.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Terrain/TerrainPatch.h>
#include <Engine/Terrain/TerrainManager.h>
#include <Engine/Physics/Types.h>
#include <Engine/Content/AssetReference.h>
#include <Engine/Content/Assets/RawDataAsset.h>
#include <Engine/Content/Content.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/TerrainTypes.h>
#include <Game/System/Game/Map.h>

API_CLASS(NoSpawn) class GAME_API TerrainDataAsset : public RawDataAsset
{
    DECLARE_BINARY_ASSET_HEADER(TerrainDataAsset, 1);
};

API_CLASS() class GAME_API LayeredTerrain : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(LayeredTerrain);
    friend class Map;

private:

    TerrainSplatmapType _splatmapType = TerrainSplatmapType::Game;
    Dictionary<TerrainSplatmapType, Array<byte>> _splatTypes;

#if USE_EDITOR
    void ColorCompare(const byte& color, byte& bestColor, byte type, byte& bestType);
    String ValidateSplatType(TerrainSplatmapType type);
    void ValidateFields(String& result);
#endif

public:

#if USE_EDITOR
    API_FUNCTION() void RebuildData();
#endif

    // This is the stuff that makes up the layers on per chunk basis. Kind of complex for an average designer
    // who simply wants to share terrain with different layers, so we hide it from them and handle on our own.
    API_FIELD() Dictionary<Int2, Array<AssetReference<Texture>>> Textures;
    API_FIELD() Dictionary<Int2, Array<AssetReference<TerrainDataAsset>>> Data;

    API_FIELD() ScriptingObjectReference<Terrain> Terrain;

    // Has to be setup manually in order to support different material presets for same layers on different levels
    API_FIELD() Dictionary<TerrainSplatmapType, AssetReference<MaterialBase>> Materials;

    // List of enabled layers per each of the splatmap type, should be in a 0 > X > 9 range to work

    API_FIELD() Array<TerrainGameType> GameTypes;
    API_FIELD() Array<TerrainMapResourcesType> MapResourcesTypes;

    // Gets current type of the splatmaps applied to the terrain
    API_PROPERTY(Attributes = "NoSerialize") TerrainSplatmapType GetSplatmapType() const;
    // Sets current splatmap type while also updating the textures for it
    API_PROPERTY(Attributes = "NoSerialize") void SetSplatmapType(TerrainSplatmapType type);

    template<class T>
    T GetTerrainType(TerrainSplatmapType type, const Vector3& position, const Vector3& direction, float distance)
    {
        T result = static_cast<T>(0);

        int32 splatmapSize = Terrain->GetChunkSize() * TerrainPatch::CHUNKS_COUNT_EDGE + 1;
        float splatmapSizeWorld = Terrain->GetPatch(0)->GetBounds().GetSize().X;

        Transform local;
        Terrain->GetTransform().WorldToLocal(Transform(position, {}), local);
        Vector3 localPosition = local.Translation;

        Int2 patchCoord((int32)(localPosition.X / splatmapSizeWorld), (int32)(localPosition.Z / splatmapSizeWorld));

        TerrainPatch* patch = Terrain->GetPatch(patchCoord);

        if (!patch)
        {
            return result;
        }

        if (!Data.ContainsKey(patchCoord))
        {
            return result;
        }

        BoundingBox bb = Terrain->GetBox();
        Vector3 totalTerrainSize = bb.GetSize();
        Vector3 terrainOffset = bb.Minimum;

        int32 x = (int32)(localPosition.X / (totalTerrainSize.X / splatmapSize));
        int32 z = (int32)(localPosition.Z / (totalTerrainSize.Z / splatmapSize));

        int32 index = z * splatmapSize + x;
        byte typeIndex = static_cast<byte>(type);
        auto chunk = Data[patchCoord][typeIndex]->Data;

        if (index < 0 || index >= chunk.Count())
        {
            return result;
        }

        byte resultIndex = chunk[index];

        return static_cast<T>(_splatTypes[type][resultIndex]);
    }

    // [Script]
    void OnEnable() override;
    void OnDisable() override;
};
