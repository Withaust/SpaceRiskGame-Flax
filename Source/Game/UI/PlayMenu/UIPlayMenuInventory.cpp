#include "UIPlayMenuInventory.h"

UIPlayMenuInventory::UIPlayMenuInventory(const SpawnParams& params)
    : Script(params)
{
    // Enable ticking OnUpdate function
    _tickUpdate = true;
}

void UIPlayMenuInventory::OnEnable()
{
    // Here you can add code that needs to be called when script is enabled (eg. register for events)
}

void UIPlayMenuInventory::OnDisable()
{
    // Here you can add code that needs to be called when script is disabled (eg. unregister from events)
}

void UIPlayMenuInventory::OnUpdate()
{
    // Here you can add code that needs to be called every frame
}
