#include "UMath.h"

// https://stackoverflow.com/a/26860684
uint64_t UMath::Pow(uint64_t value, uint64_t factor)
{
    unsigned long long temp;
    if (factor == 0)
    {
        return 1;
    }
    temp = Pow(value, factor / 2);
    if ((factor % 2) == 0)
    {
        return temp * temp;
    }
    else
    {
        if (factor > 0)
        {
            return value * temp * temp;
        }
        else
        {
            return (temp * temp) / value;
        }
    }
}

// https://stackoverflow.com/a/57459521
double UMath::Round(double value, int decimalPlaces)
{
    const double multiplier = std::pow(10.0, decimalPlaces);
    return std::ceil(value * multiplier) / multiplier;
}
