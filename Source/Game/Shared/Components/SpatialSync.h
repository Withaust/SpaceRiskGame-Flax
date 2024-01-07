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
#include <Game/System/Core/Networking.h>
/// <summary>
/// Actor script component that synchronizes the immediate info.
/// </summary>
/// <remarks>Interpolation and prediction logic based on https://www.gabrielgambetta.com/client-server-game-architecture.html.</remarks>
/// <remarks>Overall archetecture revamped in favor of https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/shared/usercmd.h</remarks>
API_CLASS() class GAME_API SpatialSync : public IComponent, public INetworkSerializable
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(SpatialSync);

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

private:

    struct BufferedItem
    {
        float Timestamp = 0.0f;
        Transform Value;
    };

    bool _bufferHasDeltas = false;
    Vector3 _lastPosition;
    Quaternion _lastRotation;
    ButtonsMask _lastButtonsMask;
    Array<BufferedItem> _buffer;
    SleepBlock _sendBlock;

public:

    API_FIELD(Attributes = "EditorOrder(10)") float TPS = 20.0f;
    API_FIELD(Attributes = "EditorOrder(20)") ScriptingObjectReference<Actor> Position;
    API_FIELD(Attributes = "EditorOrder(30)") ScriptingObjectReference<Actor> Rotation;
    API_FIELD(Attributes = "EditorOrder(40)") bool ReplicateButtons = false;
    API_FIELD(Attributes = "EditorOrder(50), NoSerialize, FlaxEngine.ReadOnly") ButtonsMask Buttons = ButtonsMask::None;

    // [Script]
    void OnEnable() override;
    void OnDisable() override;
    void InitiateUpdate();
    void OnUpdate() override;

    // [INetworkSerializable]
    void Serialize(NetworkStream* stream) override;
    void Deserialize(NetworkStream* stream) override;

    API_FUNCTION(NetworkRpc = "Server, Unreliable") void SendSync(const Vector3& position, const Quaternion& rotation, const ButtonsMask& buttons);
    API_FUNCTION(NetworkRpc = "Client, Unreliable") void RecieveSync(const Vector3& position, const Quaternion& rotation, const ButtonsMask& buttons);

private:
    void Set(const Transform& transform);
};

DECLARE_ENUM_OPERATORS(SpatialSync::ButtonsMask);
