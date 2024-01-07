// Copyright (c) 2012-2023 Wojciech Figat. All rights reserved.

// Interpolation and prediction logic based on https://www.gabrielgambetta.com/client-server-game-architecture.html
// Overall archetecture revamped in favor of https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/shared/usercmd.h

#include "SpatialSync.h"

namespace
{
    // Percentage of local error that is acceptable
    constexpr float Precision = 0.05f;

    FORCE_INLINE bool ShouldUpdate(const Vector3& currentPosition, const Quaternion& currentRotation, const Vector3& newPosition, const Quaternion& newRotation)
    {
        return Vector3::Distance(currentPosition, newPosition) > Precision ||
            Quaternion::AngleBetween(currentRotation, newRotation) > Precision;
    }
}

SpatialSync::SpatialSync(const SpawnParams& params)
    : IComponent(params)
{
    _tickUpdate = true;
}

void SpatialSync::OnEnable()
{
    // Initialize state
    _sendBlock = TPS;
    _bufferHasDeltas = false;
    _lastPosition = GetEntity() ? Position->GetPosition() : Vector3::Zero;
    _lastRotation = GetEntity() ? Rotation->GetOrientation() : Quaternion::Identity;
    _lastButtonsMask = ButtonsMask::None;
    _buffer.Clear();
}

void SpatialSync::OnDisable()
{
    _buffer.Resize(0);
}

void SpatialSync::InitiateUpdate()
{
    const auto& position = Position->GetPosition();
    const auto& rotation = Rotation->GetOrientation();
    ButtonsMask buttonsMask = ReplicateButtons ? Buttons : ButtonsMask::None;

    if (ShouldUpdate(position, rotation, _lastPosition, _lastRotation) || _lastButtonsMask != buttonsMask)
    {
        _lastPosition = position;
        _lastRotation = rotation;
        _lastButtonsMask = buttonsMask;
        SendSync(Position->GetPosition(), Rotation->GetOrientation(), buttonsMask);
    }
}

void SpatialSync::OnUpdate()
{
    if (TPS < ZeroTolerance)
    {
        return;
    }

    const NetworkObjectRole role = NetworkReplicator::GetObjectRole(this);
    if (role == NetworkObjectRole::OwnedAuthoritative)
    {
        if (USLEEP(_sendBlock))
        {
            InitiateUpdate();
        }
    }
    else
    {
        float lag = 0.0f;
        if (NetworkManager::Peer && NetworkManager::Peer->NetworkDriver)
        {
            // Use lag from the RTT between server and the client
            const auto stats = NetworkManager::Peer->NetworkDriver->GetStats();
            lag = stats.RTT / 2000.0f;
        }
        else
        {
            // Default lag is based on the network manager update rate
            const float fps = NetworkManager::NetworkFPS;
            lag = 1.0f / fps;
        }

        // Find the two authoritative positions surrounding the rendering timestamp
        const float now = Time::Update.UnscaledTime.GetTotalSeconds();
        const float gameTime = now - lag;

        // Drop older positions
        while (_buffer.Count() >= 2 && _buffer[1].Timestamp <= gameTime)
        {
            _buffer.RemoveAtKeepOrder(0);
        }

        // Interpolate between the two surrounding authoritative positions
        if (_buffer.Count() >= 2 && _buffer[0].Timestamp <= gameTime && gameTime <= _buffer[1].Timestamp)
        {
            const auto& b0 = _buffer[0];
            const auto& b1 = _buffer[1];
            Transform transform;
            const float alpha = (gameTime - b0.Timestamp) / (b1.Timestamp - b0.Timestamp);
            Transform::Lerp(b0.Value, b1.Value, alpha, transform);
            Set(transform);
        }
        else if (_buffer.Count() == 1 && _buffer[0].Timestamp <= gameTime)
        {
            Set(_buffer[0].Value);
        }
    }
}

void SpatialSync::Serialize(NetworkStream* stream)
{
    stream->Write(Position->GetPosition());
    stream->Write(Rotation->GetOrientation());

    if (ReplicateButtons)
        stream->Write(Buttons);
}

void SpatialSync::Deserialize(NetworkStream* stream)
{
    Transform transform;
    stream->Read(transform.Translation);
    stream->Read(transform.Orientation);
    Set(transform);

    if (ReplicateButtons)
    {
        stream->Read(Buttons);
    }
}

void SpatialSync::SendSync(const Vector3& position, const Quaternion& rotation, const ButtonsMask& buttons)
{
    NETWORK_RPC_IMPL(SpatialSync, SendSync, position, rotation, buttons);
    RecieveSync(position, rotation, buttons);
}

void SpatialSync::RecieveSync(const Vector3& position, const Quaternion& rotation, const ButtonsMask& buttons)
{
    NETWORK_RPC_IMPL(SpatialSync, RecieveSync, position, rotation, buttons);

    const NetworkObjectRole role = NetworkReplicator::GetObjectRole(this);
    if (role == NetworkObjectRole::OwnedAuthoritative)
    {
        return; // Ignore itself
    }

    if (ReplicateButtons)
    {
        Buttons = buttons;
    }

    Transform transform(position, rotation);
    // Add to the interpolation buffer
    if (TPS < ZeroTolerance)
    {
        Set(transform);
    }
    else
    {
        const float now = Time::Update.UnscaledTime.GetTotalSeconds();
        _buffer.Add({ now, transform });
        if (_bufferHasDeltas)
        {
            _buffer.Clear();
            _bufferHasDeltas = false;
        }
    }
}

void SpatialSync::Set(const Transform& transform)
{
    Position->SetPosition(transform.Translation);
    Rotation->SetOrientation(transform.Orientation);
}
