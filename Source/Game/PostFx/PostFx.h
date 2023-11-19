#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/NetworkRpc.h>
#include <Engine/Networking/NetworkManager.h>
#include <Engine/Graphics/RenderTask.h>

#include <Game/Shared/Defines.h>
#include <Game/PostFx/PovData.h>
#include <Game/PostFx/OutlineData.h>

API_CLASS() class GAME_API PostFx : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PostFx);
    friend class PovData;
    friend class OutlineData;

public:
    API_FIELD() static PostFx* Instance;

    API_FIELD() ScriptingObjectReference<PovData> PovData;
    API_FIELD() ScriptingObjectReference<OutlineData> OutlineData;

    void OnInitialize() override;
    void OnDeinitialize() override;
};
