#pragma once

#include <Engine/Scripting/Script.h>

API_ENUM() enum class TerrainSplatmapType : byte
{
    // Default rendering for visuals
    Game,
    // Rendering of resources
    MapResources,
    API_ENUM(Attributes = "HideInEditor")
    Max
};

DECLARE_ENUM_OPERATORS(TerrainSplatmapType);

API_ENUM() enum class TerrainGameType : byte
{
    Grass,
    DeadGrass,
    Scorched,
    API_ENUM(Attributes = "HideInEditor")
    Max
};

DECLARE_ENUM_OPERATORS(TerrainGameType);

API_ENUM() enum class TerrainMapResourcesType : byte
{
    None,
    Steel,
    Precious,
    API_ENUM(Attributes = "HideInEditor")
    Max
};

DECLARE_ENUM_OPERATORS(TerrainMapResourcesType);

const byte TerrainMaxes[static_cast<byte>(TerrainSplatmapType::Max)]{
    static_cast<byte>(TerrainGameType::Max),
    static_cast<byte>(TerrainMapResourcesType::Max)
};
