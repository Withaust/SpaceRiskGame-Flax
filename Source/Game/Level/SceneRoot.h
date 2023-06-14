#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Content/Content.h>
#include <Engine/Content/AssetInfo.h>
#include <Engine/Core/Config/GameSettings.h>

#include <Game/Shared/Defines.h>

API_CLASS(Attributes = "ContentContextMenu(\"New/Assets/DebugArgs\")")
class GAME_API DebugArgs : public ISerializable
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(DebugArgs);

    API_FIELD() String LaunchScene = TEXT("Main");
};

API_CLASS() class GAME_API SceneRoot : public Script
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(SceneRoot);
public:
    
    API_FIELD() static AssetReference<JsonAsset> EditorLaunch;

    void OnAwake() override;
};
