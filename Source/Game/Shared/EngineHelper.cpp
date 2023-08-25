#include "EngineHelper.h"

Array<char> EngineHelper::_compressBuffer;

EngineHelper::EngineHelper(const SpawnParams& params)
    : ScriptingObject(params)
{
}
