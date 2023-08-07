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
        private static ToolStripButton OriginalPauseButton;
        private static ToolStripButton OriginalFrameButton;
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
            OriginalPauseButton.Visible = true;
            OriginalFrameButton.Visible = true;
        }

        public static void OnStoppedPlaying()
        {
            PlayHostButton.Enabled = true;
            PlayClientButton.Enabled = true;
            OriginalPlayButton.Visible = false;
            OriginalPauseButton.Visible = false;
            OriginalFrameButton.Visible = false;
        }

        public static void AddCustomPlayButtons()
        {
            PlayHost = Content.Load<SpriteAtlas>(System.IO.Path.Combine(Globals.ProjectContentFolder, "Editor/Textures/UI/PlayHost.flax"));
            PlayClient = Content.Load<SpriteAtlas>(System.IO.Path.Combine(Globals.ProjectContentFolder, "Editor/Textures/UI/PlayClient.flax"));
            Editor.StateMachine.PlayingState.GameSettingsApplied += OnStartedPlaying;
            Editor.StateMachine.PlayingState.SceneRestored += OnStoppedPlaying;
            for (int i = 0; i < Editor.UI.ToolStrip.ChildrenCount; ++i)
            {
                var target = Editor.UI.ToolStrip.GetChild(i);
                if (target is not ToolStripButton)
                {
                    continue;
                }
                var Button = (ToolStripButton)target;
                if (Button == null)
                {
                    continue;
                }

                if(Button.TooltipText == "Play Game")
                {
                    OriginalPlayButton = Button;
                }
                else if(Button.Clicked == Editor.Simulation.RequestResumeOrPause)
                {
                    OriginalPauseButton = Button;
                }
                else if (Button.Clicked == Editor.Simulation.RequestPlayOneFrame)
                {
                    OriginalFrameButton = Button;
                }
            }

            OriginalPlayButton.Visible = false;
            OriginalPauseButton.Visible = false;
            OriginalFrameButton.Visible = false;
            PlayHostButton = (ToolStripButton)Editor.UI.ToolStrip.AddButton(PlayHost.FindSprite("Default"), Editor.Simulation.RequestPlayOrStopPlay).LinkTooltip("Play as a Host");
            PlayClientButton = (ToolStripButton)Editor.UI.ToolStrip.AddButton(PlayClient.FindSprite("Default"), OnClickedClient).LinkTooltip("Play as a Client");
            Editor.UI.ToolStrip.PerformLayout(true);
        }

        public static void RemoveCustomPlayButtons()
        {
            OriginalPlayButton = null;
            PlayHostButton.Dispose();
            PlayHostButton = null;
            PlayClientButton.Dispose();
            PlayClientButton = null;
        }
    };
}
#endif
