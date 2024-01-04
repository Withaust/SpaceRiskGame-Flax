#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Content/AssetReference.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/Entity.h>

API_CLASS() class GAME_API RobotRespawns : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(RobotRespawns);

private:

    bool _fired = false;

public:
    API_FIELD() static RobotRespawns* Instance;

    API_FIELD() AssetReference<Prefab> RobotPrefab;

    void OnPlayerConnected(NetworkClient* client) override;

    void OnDeinitialize() override;
};
