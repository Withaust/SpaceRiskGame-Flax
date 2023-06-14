#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Level/Actor.h>
#if USE_EDITOR
#include <Engine/Content/AssetReference.h>
#include <Engine/Content/Assets/Texture.h>
#include <Engine/Scripting/BinaryModule.h>
#include <Engine/Scripting/ManagedCLR/MClass.h>
#include <Engine/Scripting/ManagedCLR/MMethod.h>
#include <Editor/Utilities/ViewportIconsRenderer.h>
#endif

API_CLASS(Attributes = "ActorContextMenu(\"New/Entity\"), ActorToolbox(\"Other\")")
class GAME_API Entity : public Actor
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Entity);

#if USE_EDITOR
private:
    Guid iconId;
    static bool proxyAdded;
    void CheckProxy();
#endif
    //Dictionary<ScriptingTypeHandle, Component*> Component;
public:
#if USE_EDITOR
    API_FIELD() AssetReference<Texture> Icon;
    void OnDebugDrawSelected() override;
    void OnDisable() override;
#endif
    void OnEnable() override;
};
