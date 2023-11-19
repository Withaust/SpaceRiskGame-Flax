#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/UI/TextRender.h>
#include <Engine/UI/SpriteRender.h>
#include <Engine/Level/Actors/Camera.h>

#include <Game/Shared/Defines.h>
#include <Game/Level/TestTerrain.h>

API_CLASS() class GAME_API Map : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(Map);
    friend class TestTerrain;

    HashSet<TextRender*> _labels;
    HashSet<SpriteRender*> _icons;
    
public:

    API_FIELD() float MinZoom = 6000.0f;
    API_FIELD() float MinLabel = 5.0f;
    API_FIELD() float MinIcon = 500.0f;

    API_FIELD() float MaxZoom = 60000.0f;
    API_FIELD() float MaxLabel = 15.0f;
    API_FIELD() float MaxIcon = 3000.0f;

    API_FIELD() static Map* Instance;

    API_FIELD() ScriptingObjectReference<Camera> Camera;
    API_FIELD() ScriptingObjectReference<TestTerrain> Terrain;

    const HashSet<TextRender*> GetLabels();
    const HashSet<SpriteRender*> GetIcons();

    void OnDeinitialize() override;

    void StartUpdate();
    void Update();
    void StopUpdate();

    API_FUNCTION() void AddLabel(TextRender* text);
    API_FUNCTION() void RemoveLabel(TextRender* text);

    API_FUNCTION() void AddIcon(SpriteRender* icon);
    API_FUNCTION() void RemoveIcon(SpriteRender* icon);
};
