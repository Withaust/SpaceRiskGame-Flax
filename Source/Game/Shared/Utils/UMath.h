#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Core/Math/BoundingBox.h>

#include <cmath>

class UMath
{
public:
    static uint64_t Pow(uint64_t value, uint64_t factor);
    static double Round(double value, int decimalPlaces = 1);
};
