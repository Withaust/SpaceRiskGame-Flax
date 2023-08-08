// Copyright (c) 2012-2023 Wojciech Figat. All rights reserved.

#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Core/Math/Transform.h>
#include <Engine/Engine/Time.h>
#include <Engine/Level/Actor.h>
#include <Engine/Networking/NetworkManager.h>
#include <Engine/Networking/NetworkPeer.h>
#include <Engine/Networking/NetworkReplicator.h>
#include <Engine/Networking/NetworkStream.h>
#include <Engine/Networking/NetworkStats.h>
#include <Engine/Networking/INetworkDriver.h>
#include <Engine/Networking/NetworkRpc.h>
#include <Engine/Networking/INetworkSerializable.h>

#include <Game/Shared/Components/IComponent.h>

/// <summary>
/// Actor script component that synchronizes the immediate info.
/// </summary>
/// <remarks>Interpolation and prediction logic based on https://www.gabrielgambetta.com/client-server-game-architecture.html.</remarks>
/// <remarks>Overall archetecture revamped in favor of https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/shared/usercmd.h</remarks>
API_CLASS() class GAME_API ImmediateInfo : public IComponent, public INetworkSerializable
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(ImmediateInfo);

private:
    struct BufferedItem
    {
        float Timestamp = 0.0f;
        Transform Value;
    };

    bool _bufferHasDeltas = false;
    Transform _lastFrameTransform;
    Array<BufferedItem> _buffer;

public:

    API_ENUM(Attributes = "Flags") enum class ButtonsMask : uint32
    {
        None = 0,
        AttackLeft = 1 << 0,
        AttackRight = 1 << 1,
        AttackMiddle = 1 << 2,
        Jump = 1 << 3,
        Crouch = 1 << 4
    };

    API_FIELD(Attributes = "EditorOrder(10)") ScriptingObjectReference<Actor> Position;
    API_FIELD(Attributes = "EditorOrder(20)") ScriptingObjectReference<Actor> Rotation;
    API_FIELD(Attributes = "EditorOrder(30)") bool ReplicateButtons = false;
    API_FIELD(Attributes = "EditorOrder(40), NoSerialize, FlaxEngine.ReadOnly") ButtonsMask Buttons = ButtonsMask::None;

    // [Script]
    void OnEnable() override;
    void OnDisable() override;
    void OnUpdate() override;

    // [INetworkSerializable]
    void Serialize(NetworkStream* stream) override;
    void Deserialize(NetworkStream* stream) override;

private:
    void Set(const Transform& transform);
};

DECLARE_ENUM_OPERATORS(ImmediateInfo::ButtonsMask);