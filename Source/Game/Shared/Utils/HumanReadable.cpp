#include "HumanReadable.h"

// https://stackoverflow.com/a/14488941
String HumanReadable::ConvertUnits(uint64_t units, const Array<String>& suffixes)
{
    int magnitude = 0;
    double value = static_cast<double>(units);
    while (UMath::Round(value, 2) >= 1000)
    {
        value /= 1024;
        magnitude++;
    }

    if (magnitude < 0 || magnitude >= suffixes.Count())
    {
        return String::Format(TEXT("{0}{1}"), units, suffixes[0]);
    }

    return String::Format(TEXT("{0}{1}"), static_cast<uint64_t>(value), suffixes[magnitude]);
}

String HumanReadable::ConvertBytes(uint64_t bytes)
{
    Array<String> byteSuffixes = { TEXT("b"), TEXT("Kb"), TEXT("Mb"), TEXT("Gb"), TEXT("Tb"), TEXT("Pb"), TEXT("Eb"), TEXT("Zb"), TEXT("Yb") };
    return ConvertUnits(bytes, byteSuffixes);
}

StringAnsi HumanReadable::ConvertBytesAnsi(uint64_t bytes)
{
    return ConvertBytes(bytes).ToStringAnsi();
}

String HumanReadable::ConvertHertz(uint64_t hertz)
{
    Array<String> hertzSuffixes = { TEXT("Hz"), TEXT("KHz"), TEXT("MHz"), TEXT("GHz"), TEXT("THz"), TEXT("PHz"), TEXT("EHz"), TEXT("ZHz"), TEXT("YHz") };
    return ConvertUnits(hertz, hertzSuffixes);
}

StringAnsi HumanReadable::ConvertHertzAnsi(uint64_t hertz)
{
    return ConvertHertz(hertz).ToStringAnsi();
}
