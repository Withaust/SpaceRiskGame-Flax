#include "PawnInfo.h"

#include <Engine/Serialization/FileWriteStream.h>
#include <Engine/Serialization/JsonWriters.h>

PawnInfo::PawnInfo(const SpawnParams& params)
    : IComponent(params)
{
}

String PawnInfo::GetNameLocal() const
{
    if (_customName != String::Empty)
    {
        return _customName;
    }
    else if (DataPtr)
    {
        return DataPtr->Name;
    }
    else
    {
        return {};
    }
}

void PawnInfo::SetNameLocal(String value)
{
    GetActor()->SetName(value);
    _customName = value;
}
