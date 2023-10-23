#pragma once

#include <Engine/Scripting/Script.h>

API_ENUM() enum class Layers : int32
{
    AllCollision = 0,
    Static = 1,
    Level = 2,
    Dynamic = 3,
    PawnPlayer = 4,
    PawnPlayerLocal = 5,
    Pawn = 6,
    __7__ = 7,
    __8__ = 8,
    __9__ = 9,
    __10__ = 10,
    __11__ = 11,
    __12__ = 12,
    __13__ = 13,
    __14__ = 14,
    __15__ = 15,
    __16__ = 16,
    __17__ = 17,
    __18__ = 18,
    __19__ = 19,
    __20__ = 20,
    __21__ = 21,
    __22__ = 22,
    __23__ = 23,
    __24__ = 24,
    __25__ = 25,
    __26__ = 26,
    Terrain = 27,
    Sky = 28,
    Map = 29,
    Glass = 30,
    NoCollision = 31,
};