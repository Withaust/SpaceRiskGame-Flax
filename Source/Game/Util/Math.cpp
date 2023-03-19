#include "Math.h"

namespace SpaceRisk
{
    // https://stackoverflow.com/a/26860684
    uint64_t Math::Pow(uint64_t x, uint64_t y)
    {
        unsigned long long temp;
        if (y == 0)
            return 1;
        temp = Pow(x, y / 2);
        if ((y % 2) == 0) {
            return temp * temp;
        }
        else {
            if (y > 0)
                return x * temp * temp;
            else
                return (temp * temp) / x;
        }
    }

    // https://stackoverflow.com/a/57459521
    double Math::Round(double Value, int DecimalPlaces)
    {
        const double multiplier = std::pow(10.0, DecimalPlaces);
        return std::ceil(Value * multiplier) / multiplier;
    }
}
