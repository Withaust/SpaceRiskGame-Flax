#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/NetworkRpc.h>
#include <Engine/Networking/NetworkManager.h>
#include <Engine/Graphics/RenderTask.h>

#include <Game/Shared/Defines.h>
#include <Game/PostFx/PovFx.h>
#include <Game/PostFx/OutlineFx.h>

API_CLASS() class GAME_API PostFx : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PostFx);

public:
    API_FIELD() static PostFx* Instance;

    API_FIELD() ScriptingObjectReference<PovFx> PovFx;
    API_FIELD() ScriptingObjectReference<OutlineFx> OutlineFx;

    void Enable(Entity* LocalPawn);
    void Disable();
    void OnDeinitialize() override;
};
