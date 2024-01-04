#pragma once

#include <Engine/Scripting/Script.h>

API_ENUM() enum ReplicationPriority : byte
{
    // Used by systems, players and other important stuff
    High = 0,
    // Used by NPCs, level progression related objects (gates, covers, etc)
    Medium = 1,
    // Used by rarely interacted/unimportant entities
    Low = 2
};
