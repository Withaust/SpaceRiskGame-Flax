#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/NetworkManager.h>
#include <Engine/Networking/NetworkReplicator.h>
#include <Engine/Level/Level.h>
#include <Engine/Level/Scene/Scene.h>
#include <Engine/Engine/Engine.h>
#include <Engine/Networking/NetworkSettings.h>
#include <Engine/Engine/Engine.h>
#include <Engine/Platform/Window.h>
#include <Engine/Scripting/Enums.h>

#include <Game/Shared/Utils/Defines.h>
#include <Game/System/Core/LaunchArgs.h>
#include <Game/System/Core/Logger.h>

API_CLASS() class GAME_API Networking : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Networking);
    USINGLETON(Networking);

private:

    String _windowTitle;
    bool _gameStarted = false;
    bool _isHosting = false;

public:

    API_EVENT() Action GameStarting;

    void OnNetworkStateChanged();
    void OnNetworkClientConnected(NetworkClient* client);
    void OnNetworkClientDisconnected(NetworkClient* client);

    void OnInitialize() override;
    void OnDeinitialize() override;
    void OnSceneLoaded(Scene* scene) override;
    void OnSceneUnloaded(Scene* scene) override;

    void StartGame();
};
