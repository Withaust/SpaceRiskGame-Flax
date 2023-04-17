#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Content/Content.h>

#include <Game/Shared/Utils/Defines.h>

API_CLASS() class GAME_API LevelManager : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(LevelManager);
    USINGLETON(LevelManager);

private:

public:

    void OnInitialize() override;
    void OnDeinitialize() override;

    String GetLevelName();
    void LoadLevel(String scene);
};
