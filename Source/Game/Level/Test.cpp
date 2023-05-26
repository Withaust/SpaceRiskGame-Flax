#include "Test.h"

ClassA::ClassA(const SpawnParams& params)
    : Actor(params)
{
}

ClassB::ClassB(const SpawnParams& params)
    : ClassA(params)
{
}
