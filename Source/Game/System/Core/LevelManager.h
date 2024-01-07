#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Content/Content.h>
#include <Engine/Networking/NetworkRpc.h>

#include <Game/Shared/Defines.h>

API_CLASS() class GAME_API LevelManager : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(LevelManager);

private:

    String _mainScene;

public:
    API_FIELD() static LevelManager* Instance;

    void OnInitialize() override;
    void OnDeinitialize() override;

    void OnPlayerConnected(NetworkClient* client) override;

    void OnSceneLoaded(Scene* scene, const Guid& id);
    void OnSceneUnloaded(Scene* scene, const Guid& id);

    API_FUNCTION(NetworkRpc = "Client, ReliableOrdered") void RequestLoadLevel(String scene, NetworkRpcParams p);
    API_FUNCTION() void LoadLevel(String scene);
};
