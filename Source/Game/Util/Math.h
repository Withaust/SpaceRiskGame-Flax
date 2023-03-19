#pragma once

#include <Engine/Scripting/Script.h>

#include <cmath>

namespace SpaceRisk
{
    class Math
    {
    public:
        static uint64_t Pow(uint64_t x, uint64_t y);
        static double Round(double Value, int DecimalPlaces = 1);
    };
}
