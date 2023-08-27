#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Core/Delegate.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/Entity.h>

API_ENUM() enum class InteractionType
{
    None = 0,
    Open,
    Close,
    Toggle
};

struct InteractableOption
{
    InteractionType Type = InteractionType::None;
    Function<void(Entity*)> Invoke;
    bool Enabled = true;
};

API_CLASS() class GAME_API IInteractable : public IComponent
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(IInteractable);

private:

    Array<InteractableOption> _options;
    

public:

    static InteractableOption DefaultOption;

    void AddOption(InteractionType type, Function<void(Entity*)> invoke, bool enabled = true, uint32 order = 0);
    InteractableOption& GetOption(InteractionType type);
    const Array<InteractableOption>& GetOptions();
};
