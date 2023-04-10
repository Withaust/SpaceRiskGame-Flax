#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/NetworkClient.h>

/// <summary>
/// Base interface class for all systems.
/// </summary>
API_CLASS() class GAME_API ISystem : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(ISystem);

public:

    API_FUNCTION(Attributes = "NoAnimate") virtual void OnInitialize()
    {
    }

    API_FUNCTION(Attributes = "NoAnimate") virtual void OnDeinitialize()
    {
    }

    API_FUNCTION(Attributes = "NoAnimate") virtual void OnSceneLoaded(Scene* scene)
    {
    }

    API_FUNCTION(Attributes = "NoAnimate") virtual void OnSceneUnloaded(Scene* scene)
    {
    }

    API_FUNCTION(Attributes = "NoAnimate") virtual void OnPlayerConnected(NetworkClient* client)
    {
    }

    API_FUNCTION(Attributes = "NoAnimate") virtual void OnPlayerDisconnected(NetworkClient* client)
    {
    }
};
