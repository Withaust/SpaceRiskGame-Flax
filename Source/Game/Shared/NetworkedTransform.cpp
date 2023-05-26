// Copyright (c) 2012-2023 Wojciech Figat. All rights reserved.

// Interpolation and prediction logic based on https://www.gabrielgambetta.com/client-server-game-architecture.html

#include "NetworkedTransform.h"

PACK_STRUCT(struct Data
{
    uint8 LocalSpace : 1;
    uint8 HasSequenceIndex : 1;
    NetworkedTransform::ReplicationComponents Components : 9;
});

static_assert((int32)NetworkedTransform::ReplicationComponents::All == 3, "Invalid ReplicationComponents bit count for Data.");

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

NetworkedTransform::NetworkedTransform(const SpawnParams& params)
    : Script(params)
{
    // TODO: don't tick when using Default mode or with OwnedAuthoritative role to optimize cpu perf OR introduce TaskGraphSystem to batch NetworkedTransform updates over Job System
    _tickUpdate = 1;
}

void NetworkedTransform::SetSequenceIndex(uint16 value)
{
    NETWORK_RPC_IMPL(NetworkedTransform, SetSequenceIndex, value);
    _currentSequenceIndex = value;
}

void NetworkedTransform::OnEnable()
{
    // Initialize state
    _bufferHasDeltas = false;
    _currentSequenceIndex = 0;
    _lastFrameTransform = GetActor() ? GetActor()->GetTransform() : Transform::Identity;
    _buffer.Clear();
}

void NetworkedTransform::OnDisable()
{
    _buffer.Resize(0);
}

void NetworkedTransform::OnUpdate()
{
    // TODO: cache role in Deserialize to improve cpu perf
    const NetworkObjectRole role = NetworkReplicator::GetObjectRole(this);
    if (role == NetworkObjectRole::OwnedAuthoritative)
    {
        return; // Ignore itself
    }
    if (Mode == ReplicationModes::Default)
    {
        // Transform replicated in Deserialize
    }
    else if (role == NetworkObjectRole::ReplicatedSimulated && Mode == ReplicationModes::Prediction)
    {
        // Compute delta of the actor transformation simulated locally
        const Transform thisFrameTransform = GetActor() ? GetActor()->GetTransform() : Transform::Identity;
        Transform delta = thisFrameTransform - _lastFrameTransform;

        if (!delta.IsIdentity())
        {
            // Move to the next input sequence number
            _currentSequenceIndex++;

            // Add delta to buffer to re-apply after receiving authoritative transform value
            if (!_bufferHasDeltas)
            {
                _buffer.Clear();
                _bufferHasDeltas = true;
            }
            delta.Orientation = thisFrameTransform.Orientation; // Store absolute orientation value to prevent jittering when blending rotation deltas
            _buffer.Add({ 0.0f, _currentSequenceIndex, delta, });

            // Inform server about sequence number change (add offset to lead before server data)
            SetSequenceIndex(_currentSequenceIndex - 1);
        }
        _lastFrameTransform = thisFrameTransform;
    }
    else
    {
        float lag = 0.0f;
        // TODO: use lag from last used NetworkStream context
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

void NetworkedTransform::Serialize(NetworkStream* stream)
{
    // Get transform
    Transform transform(Position->GetPosition(), Rotation->GetOrientation());

    // Encode data
    const NetworkObjectRole role = NetworkReplicator::GetObjectRole(this);
    Data data;
    data.HasSequenceIndex = Mode == ReplicationModes::Prediction;
    data.Components = Components;
    stream->Write(data);
    if (EnumHasAllFlags(data.Components, ReplicationComponents::All))
    {
        stream->Write(transform);
    }
    else
    {
        if (EnumHasAllFlags(data.Components, ReplicationComponents::Position))
        {
            stream->Write(transform.Translation);
        }
        if (EnumHasAllFlags(data.Components, ReplicationComponents::Rotation))
        {
            const Float3 rotation = transform.Orientation.GetEuler();
            stream->Write(rotation);
        }
    }
    if (data.HasSequenceIndex)
    {
        stream->Write(_currentSequenceIndex);
    }
}

void NetworkedTransform::Deserialize(NetworkStream* stream)
{
    // Get transform
    Transform transform(Position->GetPosition(), Rotation->GetOrientation());
    Transform transformLocal = transform;

    // Decode data
    Data data;
    stream->Read(data);
    if (EnumHasAllFlags(data.Components, ReplicationComponents::All))
    {
        stream->Read(transform);
    }
    else
    {
        if (EnumHasAllFlags(data.Components, ReplicationComponents::Position))
        {
            stream->Read(transform.Translation);
        }
        if (EnumHasAllFlags(data.Components, ReplicationComponents::Rotation))
        {
            Float3 rotation;
            stream->Read(rotation);
            transform.Orientation = Quaternion::Euler(rotation);
        }
    }
    uint16 sequenceIndex = 0;
    if (data.HasSequenceIndex)
    {
        stream->Read(sequenceIndex);
    }

    const NetworkObjectRole role = NetworkReplicator::GetObjectRole(this);
    if (role == NetworkObjectRole::OwnedAuthoritative)
    {
        return; // Ignore itself
    }
    if (Mode == ReplicationModes::Default)
    {
        // Immediate set
        Set(transform);
    }
    else if (role == NetworkObjectRole::ReplicatedSimulated && Mode == ReplicationModes::Prediction)
    {
        const Transform transformAuthoritative = transform;
        const Transform transformDeltaBefore = transformAuthoritative - transformLocal;

        // Remove any transform deltas from local simulation that happened before the incoming authoritative transform data
        if (!_bufferHasDeltas)
        {
            _buffer.Clear();
            _bufferHasDeltas = true;
        }
        // TODO: items are added in order to do batch removal
        for (int32 i = 0; i < _buffer.Count() && _buffer[i].SequenceIndex < sequenceIndex; i++)
        {
            _buffer.RemoveAtKeepOrder(i);
        }

        // Use received authoritative actor transformation but re-apply all deltas not yet processed by the server due to lag (reconciliation)
        for (auto& e : _buffer)
        {
            transform.Translation = transform.Translation + e.Value.Translation;
        }
        // TODO: use euler angles or similar to cache/reapply rotation deltas (Quaternion jitters)
        transform.Orientation = transformLocal.Orientation;

        // If local simulation is very close to the authoritative server value then ignore slight error (based relative delta threshold)
        const Transform transformDeltaAfter = transformAuthoritative - transform;
        if (IsWithinPrecision(transformDeltaBefore.Translation, transformDeltaAfter.Translation))
        {
            return;
        }

        // Set to the incoming value with applied local deltas
        Set(transform);
        _lastFrameTransform = transform;
    }
    else
    {
        // Add to the interpolation buffer
        const float now = Time::Update.UnscaledTime.GetTotalSeconds();
        _buffer.Add({ now, 0, transform });
        if (_bufferHasDeltas)
        {
            _buffer.Clear();
            _bufferHasDeltas = false;
        }
    }
}

void NetworkedTransform::Set(const Transform& transform)
{
    Position->SetPosition(transform.Translation);
    Rotation->SetOrientation(transform.Orientation);
}
