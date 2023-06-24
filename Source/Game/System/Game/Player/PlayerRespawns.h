#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Prefabs/PrefabManager.h>
#include <Engine/Level/Prefabs/Prefab.h>
#include <Engine/Content/SoftAssetReference.h>
#include <Engine/Networking/NetworkManager.h>
#include <Engine/Core/Collections/HashSet.h>
#include <Engine/Core/RandomStream.h>

#include <Game/Shared/Defines.h>
#include <Game/System/Core/Networking.h>
#include <Game/Dynamic/Player/PlayerNetworking.h>
#include <Game/Level/Player/PlayerSpawn.h>

API_CLASS() class GAME_API PlayerRespawns : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PlayerRespawns);

    friend class PlayerSpawn;

private:

    Array<ScriptingObjectReference<PlayerSpawn>> _spawns;

public:
    API_FIELD() static PlayerRespawns* Instance;

    API_FUNCTION() ScriptingObjectReference<PlayerSpawn> GetRandomSpawn();

    API_FUNCTION() void Register(ScriptingObjectReference<PlayerSpawn> spawn);
    API_FUNCTION() void Unregister(ScriptingObjectReference<PlayerSpawn> spawn);
};
