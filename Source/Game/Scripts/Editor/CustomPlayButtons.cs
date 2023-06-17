#if FLAX_EDITOR
using FlaxEditor;
using FlaxEditor.GUI;
using FlaxEditor.States;
using FlaxEngine;

namespace Game
{
    public class CustomPlayButtons
    {
        private static SpriteAtlas PlayHost;
        private static SpriteAtlas PlayClient;
        private static ToolStripButton OriginalPlayButton;
        private static ToolStripButton PlayHostButton;
        private static ToolStripButton PlayClientButton;

        public static Editor Editor;

        public static void OnClickedClient()
        {
            LaunchArgs.ForceClient = true;
            Editor.Simulation.RequestPlayOrStopPlay();
        }

        public static void OnStartedPlaying()
        {
            PlayHostButton.Enabled = false;
            PlayClientButton.Enabled = false;
            OriginalPlayButton.Visible = true;
        }

        public static void OnStoppedPlaying()
        {
            PlayHostButton.Enabled = true;
            PlayClientButton.Enabled = true;
            OriginalPlayButton.Visible = false;
        }

        public static void AddCustomPlayButtons()
        {
            PlayHost = Content.Load<SpriteAtlas>(System.IO.Path.Combine(Globals.ProjectContentFolder, "Editor/Textures/UI/PlayHost.flax"));
            PlayClient = Content.Load<SpriteAtlas>(System.IO.Path.Combine(Globals.ProjectContentFolder, "Editor/Textures/UI/PlayClient.flax"));
            Editor.StateMachine.PlayingState.GameSettingsApplied += OnStartedPlaying;
            Editor.StateMachine.PlayingState.SceneRestored += OnStoppedPlaying;
            OriginalPlayButton = null;
            int PlayIndex = 0;
            for (int i = 0; i < Editor.UI.ToolStrip.ChildrenCount; ++i)
            {
                var target = Editor.UI.ToolStrip.GetChild(i);
                if (!(target is ToolStripButton))
                {
                    continue;
                }
                OriginalPlayButton = (ToolStripButton)target;
                if (OriginalPlayButton == null || OriginalPlayButton.Clicked != Editor.Simulation.RequestPlayOrStopPlay)
                {
                    continue;
                }
                PlayIndex = i;
                break;
            }

            OriginalPlayButton.Visible = false;
            PlayHostButton = (ToolStripButton)Editor.UI.ToolStrip.AddButton(PlayHost.FindSprite("Default"), Editor.Simulation.RequestPlayOrStopPlay).LinkTooltip("Play as a Host");
            PlayClientButton = (ToolStripButton)Editor.UI.ToolStrip.AddButton(PlayClient.FindSprite("Default"), OnClickedClient).LinkTooltip("Play as a Client");
            Editor.UI.ToolStrip.PerformLayout(true);
        }

        public static void RemoveCustomPlayButtons()
        {
            for (int i = 0; i < Editor.UI.ToolStrip.ChildrenCount; ++i)
            {
                var target = Editor.UI.ToolStrip.GetChild(i);
                if (!(target is ToolStripButton))
                {
                    continue;
                }
                ToolStripButton targetButton = (ToolStripButton)target;
                if (targetButton == null || !targetButton.TooltipText.StartsWith("Play as a"))
                {
                    continue;
                }
                Editor.UI.ToolStrip.RemoveChild(targetButton);
                break;
            }
        }
    };
}
#endif
