/*

Causes strainge crashes when trying to offload different prefabs, better not use for now

#if FLAX_EDITOR
using FlaxEditor;
using FlaxEditor.GUI;
using FlaxEngine;
using System.Collections.Generic;
using System.Linq;

public class PlayButton : EditorPlugin
{
    private List<Scene> _scenes;

    public void OnSceneUnloaded(Scene scene, System.Guid id)
    {
        if (Level.ScenesCount == 0)
        {
            Level.SceneUnloaded -= OnSceneUnloaded;
            Level.UnloadAllScenesAsync();
            for (int i = 0; i < _scenes.Count; ++i)
            {
                Level.LoadSceneAsync(_scenes[i].ID);
            }
            _scenes.Clear();
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
            _scenes = Level.Scenes.ToList();
            Level.UnloadAllScenes();
            AssetInfo info;
            Content.GetAssetInfo("Content/Levels/Core.scene", out info);
            Level.LoadScene(info.ID);
        }
        Editor.Simulation.RequestPlayOrStopPlay();
    }

    public override void InitializeEditor()
    {
        base.InitializeEditor();

        //Disable key input, since it brings hooking issues
        Editor.Options.Options.Input.Play = new FlaxEditor.Options.InputBinding(KeyboardKeys.None);

        ToolStripButton play = null;
        for (int i = 0; i < Editor.UI.ToolStrip.ChildrenCount; ++i)
        {
            var target = Editor.UI.ToolStrip.GetChild(i);
            if (!(target is ToolStripButton))
            {
                continue;
            }
            play = (ToolStripButton)target;
            if (play == null || play.Clicked != Editor.Simulation.RequestPlayOrStopPlay)
            {
                continue;
            }
            break;
        }
        play.LinkTooltip("Start/Stop the game");
        play.Clicked = OnClicked;
    }

    public override void Deinitialize()
    {
        base.Deinitialize();
    }
}
#endif
*/
