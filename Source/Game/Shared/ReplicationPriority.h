#pragma once

#include <Engine/Scripting/Script.h>

API_ENUM() enum ReplicationPriority : byte
{
    // Used by real-time moving stuff like players, NPCs, etc
    High = 0,
    // Used by level related objects (gates, doors, covers, etc)
    Medium = 1,
    // Used by rarely interacted/unimportant entities
    Low = 2,
    API_ENUM(Attributes = "HideInEditor")
    Max
};
