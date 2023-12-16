#include "EngineHelper.h"

Array<byte> EngineHelper::_compressBuffer;

EngineHelper::EngineHelper(const SpawnParams& params)
    : ScriptingObject(params)
{
}
