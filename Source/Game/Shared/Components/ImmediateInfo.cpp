// Copyright (c) 2012-2023 Wojciech Figat. All rights reserved.

// Interpolation and prediction logic based on https://www.gabrielgambetta.com/client-server-game-architecture.html
// Overall archetecture revamped in favor of https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/shared/usercmd.h

#include "ImmediateInfo.h"

namespace
{
    // Percentage of local error that is acceptable (eg. 4 frames error)
    constexpr float Precision = 8.0f;

    template<typename T>
    FORCE_INLINE bool IsWithinPrecision(const Vector3Base<T>& currentDelta, const Vector3Base<T>& targetDelta)
    {
        const T targetDeltaMax = targetDelta.GetAbsolute().MaxValue();
        return targetDeltaMax > (T)ZeroTolerance && currentDelta.GetAbsolute().MaxValue() < targetDeltaMax * (T)Precision;
    }
}

ImmediateInfo::ImmediateInfo(const SpawnParams& params)
    : IComponent(params)
{
    _tickUpdate = true;
}

void ImmediateInfo::OnEnable()
{
    // Initialize state
    _bufferHasDeltas = false;
    _lastFrameTransform = GetEntity() ? Transform(Position->GetPosition(), Rotation->GetOrientation()) : Transform::Identity;
    _buffer.Clear();
}

void ImmediateInfo::OnDisable()
{
    _buffer.Resize(0);
}

void ImmediateInfo::OnUpdate()
{
    const NetworkObjectRole role = NetworkReplicator::GetObjectRole(this);
    if (role == NetworkObjectRole::OwnedAuthoritative)
    {
        return; // Ignore itself
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

void ImmediateInfo::Serialize(NetworkStream* stream)
{
    // Get transform
    Transform transform(Position->GetPosition(), Rotation->GetOrientation());

    stream->Write(transform);
    if(ReplicateButtons)
        stream->Write(Buttons);
}

void ImmediateInfo::Deserialize(NetworkStream* stream)
{
    // Get transform
    Transform transform(Position->GetPosition(), Rotation->GetOrientation());

    stream->Read(transform);
    if (ReplicateButtons)
        stream->Read(Buttons);

    const NetworkObjectRole role = NetworkReplicator::GetObjectRole(this);
    if (role == NetworkObjectRole::OwnedAuthoritative)
    {
        return; // Ignore itself
    }

    // Add to the interpolation buffer
    const float now = Time::Update.UnscaledTime.GetTotalSeconds();
    _buffer.Add({ now, transform });
    if (_bufferHasDeltas)
    {
        _buffer.Clear();
        _bufferHasDeltas = false;
    }
}

void ImmediateInfo::Set(const Transform& transform)
{
    Position->SetPosition(transform.Translation);
    Rotation->SetOrientation(transform.Orientation);
}
