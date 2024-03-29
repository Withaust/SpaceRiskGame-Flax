#include "PlayerRespawns.h"

UIMPL_SINGLETON(PlayerRespawns)

PlayerRespawns::PlayerRespawns(const SpawnParams& params)
    : ISystem(params)
{
}

ScriptingObjectReference<PlayerSpawn> PlayerRespawns::GetRandomSpawn()
{
    if (_spawns.Count() == 0)
    {
        UCRIT(true, "Missing respawns for players.");
        return {};
    }
    RandomStream random;
    random.GenerateNewSeed();
    int index = random.RandRange(0, _spawns.Count() - 1);
    return _spawns[index];
}

void PlayerRespawns::Register(ScriptingObjectReference<PlayerSpawn> spawn)
{
    if (spawn)
    {
        _spawns.Add(spawn);
    }
}

void PlayerRespawns::Unregister(ScriptingObjectReference<PlayerSpawn> spawn)
{
    if (spawn)
    {
        _spawns.Remove(spawn);
    }
}

void PlayerRespawns::OnDeinitialize()
{
    UDEINIT_SINGLETON();
}