#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Core/Collections/Dictionary.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Terrain/TerrainPatch.h>
#include <Engine/Terrain/TerrainManager.h>
#include <Engine/Physics/Types.h>

#include <Game/Shared/Defines.h>
#include <Game/System/Game/Map.h>

API_CLASS() class GAME_API TestTerrain : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(TestTerrain);
    friend class Map;

public:

    API_ENUM() enum class TerrainType : byte
    {
        Grass = 0,
        DeadGrass = 1,
        Scorched = 2,
        API_ENUM(Attributes = "HideInEditor")
        Max
    };

    API_ENUM() enum class SplatmapType : byte
    {
        Game = 0,
        MapResources = 1,
        API_ENUM(Attributes = "HideInEditor")
        Max
    };

private:

    Dictionary<Int2, Array<TerrainType>> _cachedChunks;
    TestTerrain::SplatmapType _splatmapType = TestTerrain::SplatmapType::Game;

public:

    API_FIELD() ScriptingObjectReference<Terrain> Terrain;

    API_PROPERTY() TestTerrain::SplatmapType GetSplatmap() const
    {
        return _splatmapType;
    }

    API_PROPERTY() void SetSplatmap(const TestTerrain::SplatmapType& type)
    {
#if USE_EDITOR
        String error = TEXT("");
        if (type == SplatmapType::Max)
        {
            error = TEXT("Cant use SplatmapType::Max");
        }
        for (const auto& patch : SplatMaps)
        {
            if (!error.IsEmpty())
            {
                break;
            }
            const auto& textures = patch.Value;
            if (textures.Count() != static_cast<int>(SplatmapType::Max) * 2)
            {
                error = TEXT("Assigned texture count is wrong");
                break;
            }
            for (const auto& texture : textures)
            {
                if (!texture)
                {
                    error = TEXT("Some textures were not assigned");
                    break;
                }
            }
        }
        for (const auto& material : SplatMapMaterials)
        {
            if (!material.Value)
            {
                error = TEXT("Some materials were not assigned");
                break;
            }
        }
        if (!error.IsEmpty())
        {
            UCRIT_STR(true, error);
        }
#endif
        for (const auto& patch : SplatMaps)
        {
            const auto& textures = patch.Value;
            int index = static_cast<int>(type);
            Array<String> split;
            patch.Key.Split('x', split);
            int x = 0;
            int y = 0;
            StringUtils::Parse(split[0].Get(), &x);
            StringUtils::Parse(split[1].Get(), &y);
            Int2 key(x, y);
            Terrain->GetPatch(key)->SetSplatmap(0, textures[index * 2]);
            Terrain->GetPatch(key)->SetSplatmap(1, textures[(index * 2) + 1]);
            Terrain->GetPatch(key)->ClearSplatMapCache();
        }
        Terrain->Material = SplatMapMaterials[type];
        _splatmapType = type;
    }

    API_FIELD() Dictionary<String, Array<AssetReference<Texture>>> SplatMaps;
    API_FIELD() Dictionary<TestTerrain::SplatmapType, AssetReference<MaterialBase>> SplatMapMaterials;

    API_FIELD() Dictionary<uint8, TestTerrain::TerrainType> TerrainTypes;

    API_FUNCTION() TerrainType GetTerrainType(const Vector3& position, const Vector3& direction, float distance);

    // [Script]
    void OnEnable() override;
    void OnDisable() override;
};

DECLARE_ENUM_OPERATORS(TestTerrain::TerrainType);
DECLARE_ENUM_OPERATORS(TestTerrain::SplatmapType);
