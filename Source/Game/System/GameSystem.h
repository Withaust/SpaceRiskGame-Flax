#pragma once

#include <Engine/Scripting/Script.h>

API_CLASS() class GAME_API GameSystem : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(GameSystem);
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
};
