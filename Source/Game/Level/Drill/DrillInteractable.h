#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actor.h>
#include <Engine/UI/TextRender.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/Components/IInteractable.h>
#include <Game/Level/Drill/Drill.h>

API_CLASS() class GAME_API DrillInteractable : public IInteractable
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(DrillInteractable);

private:

    void OnClose(Entity* entity);

public:

    void OnEnable() override;
};
