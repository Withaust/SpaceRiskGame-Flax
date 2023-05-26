#include "PlayerRespawns.h"

Array<ScriptingObjectReference<PlayerSpawn>> PlayerRespawns::_spawns;

PlayerRespawns::PlayerRespawns(const SpawnParams& params)
    : ISystem(params)
{
}

ScriptingObjectReference<PlayerSpawn> PlayerRespawns::GetRandomSpawn()
{
    if (_spawns.Count() == 0)
    {
        UCRIT_STR(true, "Missing respawns for players.");
        return {};
    }
    RandomStream random;
    random.GenerateNewSeed();
    int index = random.RandRange(0, _spawns.Count() - 1);
    return _spawns[index];
}

void PlayerRespawns::Register(ScriptingObjectReference<PlayerSpawn> spawn)
{
    _spawns.Add(spawn);
}

void PlayerRespawns::Unregister(ScriptingObjectReference<PlayerSpawn> spawn)
{
    _spawns.Remove(spawn);
}
