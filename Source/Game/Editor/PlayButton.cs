/*
#if FLAX_EDITOR
using FlaxEditor;
using FlaxEditor.GUI;
using FlaxEngine;
using System.Collections.Generic;
using System.Linq;

public class PlayButton : EditorPlugin
{
    private List<Scene> Scenes;

    public void OnSceneUnloaded(Scene Scene, System.Guid Id)
    {
        if(Level.ScenesCount == 0)
        {
            Level.SceneUnloaded -= OnSceneUnloaded;
            Level.UnloadAllScenesAsync();
            for (int i = 0; i < Scenes.Count; ++i)
            {
                Level.LoadSceneAsync(Scenes[i].ID);
            }
            Scenes.Clear();
        }
    }

    public void OnClicked()
    {
        if (Editor.StateMachine.IsPlayMode)
        {
            Level.SceneUnloaded += OnSceneUnloaded;
        }
        else
        {
            //Remove scenes and load core
            Level.SaveAllScenes();
            Scenes = Level.Scenes.ToList();
            Level.UnloadAllScenes();
            AssetInfo Info;
            Content.GetAssetInfo("Content/Levels/Core.scene", out Info);
            Level.LoadScene(Info.ID);
        }
        Editor.Simulation.RequestPlayOrStopPlay();
    }

    public override void InitializeEditor()
    {
        base.InitializeEditor();

        //Disable key input, since it brings hooking issues
        Editor.Options.Options.Input.Play = new FlaxEditor.Options.InputBinding(KeyboardKeys.None);

        ToolStripButton Play = null;
        for (int i = 0; i < Editor.UI.ToolStrip.ChildrenCount; ++i)
        {
            var Target = Editor.UI.ToolStrip.GetChild(i);
            if (!(Target is ToolStripButton))
            {
                continue;
            }
            Play = (ToolStripButton)Target;
            if (Play == null || Play.Clicked != Editor.Simulation.RequestPlayOrStopPlay)
            {
                continue;
            }
            break;
        }
        Play.LinkTooltip("Start/Stop the game");
        Play.Clicked = OnClicked;
    }

    public override void Deinitialize()
    {
        base.Deinitialize();
    }
}
#endif
*/