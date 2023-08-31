#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actor.h>
#include <Engine/Level/Actors/SpotLight.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/Components/IInteractable.h>

API_CLASS() class GAME_API Button : public IInteractable
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Button);

private:

    void OnToggle(Entity* entity);

    bool _state = false;

public:

    API_PROPERTY(NetworkReplicated) bool GetStateLocal() const { UIMPL_NETPROP_GETLOCAL(_state); }
    API_PROPERTY(NetworkReplicated) void SetStateLocal(bool value);
    API_FUNCTION(NetworkRpc = "Server, Reliable") void SetStateRemote(float value) { UIMPL_NETPROP_SETREMOTE(Button, State); }
    API_FUNCTION(NetworkRpc = "Client, Reliable") void SetStateSync(float value) { UIMPL_NETPROP_SETSYNC(Button, State); }

    API_FIELD() Array<ScriptingObjectReference<SpotLight>> Lights;

    void OnEnable() override;
};
