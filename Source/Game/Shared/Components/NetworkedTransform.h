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

/// <summary>
/// Actor script component that synchronizes the Transform over the network.
/// </summary>
/// <remarks>Interpolation and prediction logic based on https://www.gabrielgambetta.com/client-server-game-architecture.html.</remarks>
API_CLASS() class GAME_API NetworkedTransform : public Script, public INetworkSerializable
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(NetworkedTransform);

    /// <summary>
    /// Actor transform replication components (flags).
    /// </summary>
    API_ENUM(Attributes = "Flags") enum class ReplicationComponents
    {
        // No sync.
        None = 0,

        // Position X component.
        PositionX = 1 << 0,
        // Position Y component.
        PositionY = 1 << 1,
        // Position Z component.
        PositionZ = 1 << 2,
        // Position XYZ components (full).
        Position = PositionX | PositionY | PositionZ,

        // Scale X component.
        ScaleX = 1 << 3,
        // Scale Y component.
        ScaleY = 1 << 4,
        // Scale Z component.
        ScaleZ = 1 << 5,
        // Scale XYZ components (full).
        Scale = ScaleX | ScaleY | ScaleZ,

        // Position X component.
        RotationX = 1 << 6,
        // Position Y component.
        RotationY = 1 << 7,
        // Position Z component.
        RotationZ = 1 << 8,
        // Rotation XYZ components (full).
        Rotation = RotationX | RotationY | RotationZ,

        // All components fully synchronized.
        All = Position | Scale | Rotation,
    };

    /// <summary>
    /// Actor transform replication modes.
    /// </summary>
    API_ENUM() enum class ReplicationModes
    {
        // The transform replicated from the owner (raw replication data messages that might result in sudden object jumps when moving).
        Default,
        // The transform replicated from the owner with local interpolation between received data to provide smoother movement.
        Interpolation,
        // The transform replicated from the owner but with local prediction (eg. player character that has local simulation but is validated against authoritative server).
        Prediction,
    };

private:
    struct BufferedItem
    {
        float Timestamp;
        uint16 SequenceIndex;
        Transform Value;
    };

    bool _bufferHasDeltas;
    uint16 _currentSequenceIndex = 0;
    Transform _lastFrameTransform;
    Array<BufferedItem> _buffer;

public:

    /// <summary>
    /// Actor transform replication components (flags).
    /// </summary>
    API_FIELD(Attributes = "EditorOrder(10)")
        ReplicationComponents Components = ReplicationComponents::All;

    /// <summary>
    /// Actor transform replication mode.
    /// </summary>
    API_FIELD(Attributes = "EditorOrder(20)")
        ReplicationModes Mode = ReplicationModes::Default;

    /// <summary>
    /// Target position to transform.
    /// </summary>
    API_FIELD(Attributes = "EditorOrder(30)")
        ScriptingObjectReference<Actor> Position;

    /// <summary>
    /// Target rotation to transform.
    /// </summary>
    API_FIELD(Attributes = "EditorOrder(40)")
        ScriptingObjectReference<Actor> Rotation;

private:
    API_FUNCTION(Hidden, NetworkRpc = Server) void SetSequenceIndex(uint16 value);

public:
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

DECLARE_ENUM_OPERATORS(NetworkedTransform::ReplicationComponents);
