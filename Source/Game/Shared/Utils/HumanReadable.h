#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Core/Math/Math.h>

#include <Game/Shared/Utils/UMath.h>

class HumanReadable
{
public:
    static String ConvertUnits(uint64_t units, const Array<String>& suffixes);
    static String ConvertBytes(uint64_t bytes);
    static StringAnsi ConvertBytesAnsi(uint64_t bytes);
    static String ConvertHertz(uint64_t hertz);
    static StringAnsi ConvertHertzAnsi(uint64_t hertz);
};
