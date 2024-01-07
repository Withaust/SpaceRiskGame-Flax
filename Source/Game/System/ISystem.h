#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/NetworkClient.h>

#include <Game/Shared/INetworkedObject.h>

/// <summary>
/// Base interface class for all systems.
/// </summary>
API_CLASS() class GAME_API ISystem : public INetworkedObject
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(ISystem);
    friend class Core;
    friend class Networking;

public:

    /// <summary>
    /// Executes upon initial initialization of all systems in the order of their addition in CoreInstance.cpp
    /// </summary>
    API_FUNCTION(Attributes = "NoAnimate") virtual void OnInitialize()
    {
    }

    /// <summary>
    /// Executes upon final deinitialization of all systems in backwards order of their addition in CoreInstance.cpp
    /// </summary>
    API_FUNCTION(Attributes = "NoAnimate") virtual void OnDeinitialize()
    {
    }

    /// <summary>
    /// Executes upon loading of a sceme on all systems (loading of Core is ignored)
    /// </summary>
    API_FUNCTION(Attributes = "NoAnimate") virtual void OnSceneLoaded(Scene* scene)
    {
    }

    /// <summary>
    /// Executes upon unloading of a sceme on all systems (unloading of Core is ignored)
    /// </summary>
    API_FUNCTION(Attributes = "NoAnimate") virtual void OnSceneUnloaded(Scene* scene)
    {
    }

    /// <summary>
    /// Executes when we finally connect to the server (this gets fired on both host and client as well)
    /// </summary>
    API_FUNCTION(Attributes = "NoAnimate") virtual void OnConnected()
    {
    }

    /// <summary>
    /// Executes when we get disconnected from the server (this gets fired on both host and client as well)
    /// </summary>
    API_FUNCTION(Attributes = "NoAnimate") virtual void OnDisconnected()
    {
    }

    /// <summary>
    /// Executes when a client gets connected to a server (this only fires on host)
    /// </summary>
    API_FUNCTION(Attributes = "NoAnimate") virtual void OnPlayerConnected(NetworkClient* client)
    {
    }

    /// <summary>
    /// Executes when a client loads on the level after connected to a server (this only fires on host)
    /// </summary>
    API_FUNCTION(Attributes = "NoAnimate") virtual void OnPlayerLoaded(NetworkClient* client)
    {
    }

    /// <summary>
    /// Executes when a client gets disconnected from a server (this only fires on host)
    /// </summary>
    API_FUNCTION(Attributes = "NoAnimate") virtual void OnPlayerDisconnected(NetworkClient* client)
    {
    }
};
