#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Core/Collections/Dictionary.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Terrain/TerrainPatch.h>
#include <Engine/Terrain/TerrainManager.h>
#include <Engine/Physics/Types.h>

#include <Game/Shared/Defines.h>

API_CLASS() class GAME_API TestTerrain : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(TestTerrain);

public:

    API_ENUM() enum class TerrainType : byte
    {
        Grass = 1,
        DeadGrass = 2,
        Scorched = 3,
    };

private:

    Terrain* _terrain = nullptr;
    Dictionary<Int2, Array<TerrainType>> _cachedChunks;

public:

    API_FIELD() Dictionary<uint8, TestTerrain::TerrainType> TerrainTypes;

    API_FUNCTION() TerrainType GetTerrainType(const Vector3& position, const Vector3& direction, float distance);

    // [Script]
    void OnEnable() override;
    void OnDisable() override;
};

//DECLARE_ENUM_OPERATORS(TestTerrain::TerrainType);
