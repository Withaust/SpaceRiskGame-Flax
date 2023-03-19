﻿#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Core/Math/Math.h>

#include <Game/Util/Math.h>

class HumanReadable 
{
public:
    static String ConvertUnits(uint64_t Units, const Array<String>& Suffixes);
    static String ConvertBytes(uint64_t Bytes);
    static String ConvertHertz(uint64_t Hertz);
};
