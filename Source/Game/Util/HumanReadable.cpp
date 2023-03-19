#include "HumanReadable.h"

// Port from C# answer https://stackoverflow.com/a/14488941
String HumanReadable::ConvertUnits(uint64_t Units, const Array<String>& Suffixes)
{
    int Magnitude = 0;
    double dValue = static_cast<double>(Units);
    while (SpaceRisk::Math::Round(dValue, 2) >= 1000)
    {
        dValue /= 1024;
        Magnitude++;
    }

    return String::Format(TEXT("{0}{1}"), static_cast<uint64_t>(dValue), Suffixes[Magnitude]);
}

String HumanReadable::ConvertBytes(uint64_t Bytes)
{
    Array<String> ByteSuffixes = { TEXT("b"), TEXT("Kb"), TEXT("Mb"), TEXT("Gb"), TEXT("Tb"), TEXT("Pb"), TEXT("Eb"), TEXT("Zb"), TEXT("Yb") };
    return ConvertUnits(Bytes, ByteSuffixes);
}

String HumanReadable::ConvertHertz(uint64_t Hertz)
{
    Array<String> HertzSuffixes = { TEXT("Hz"), TEXT("KHz"), TEXT("MHz"), TEXT("GHz"), TEXT("THz"), TEXT("PHz"), TEXT("EHz"), TEXT("ZHz"), TEXT("YHz") };
    return ConvertUnits(Hertz, HertzSuffixes);
}

