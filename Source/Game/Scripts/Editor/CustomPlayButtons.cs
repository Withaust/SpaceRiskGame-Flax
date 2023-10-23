#if FLAX_EDITOR
using FlaxEditor;
using FlaxEditor.GUI;
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

        public static void OnClickedHost()
        {
            LaunchArgs.IsHost = true;
            LaunchArgs.Hostname = "";
            if (!LaunchArgs.IsSteam)
            {
                LaunchArgs.Name = "Host";
            }
            Editor.Instance.Simulation.RequestPlayScenesOrStopPlay();
        }

        public static void OnClickedClient()
        {
            LaunchArgs.IsHost = false;
            LaunchArgs.Hostname = "localhost";
            if (!LaunchArgs.IsSteam)
            {
                LaunchArgs.Name = "Client";
            }
            Editor.Instance.Simulation.RequestPlayScenesOrStopPlay();
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
            Editor.Instance.StateMachine.PlayingState.GameSettingsApplied += OnStartedPlaying;
            Editor.Instance.StateMachine.PlayingState.SceneRestored += OnStoppedPlaying;
            for (int i = 0; i < Editor.Instance.UI.ToolStrip.ChildrenCount; ++i)
            {
                var target = Editor.Instance.UI.ToolStrip.GetChild(i);
                if (target is not ToolStripButton)
                {
                    continue;
                }
                var Button = (ToolStripButton)target;
                if (Button == null)
                {
                    continue;
                }

                if (Button.TooltipText == "Play In Editor (F5)")
                {
                    OriginalPlayButton = Button;
                }
                else if (Button.Clicked == Editor.Instance.Simulation.RequestResumeOrPause)
                {
                    OriginalPauseButton = Button;
                }
                else if (Button.Clicked == Editor.Instance.Simulation.RequestPlayOneFrame)
                {
                    OriginalFrameButton = Button;
                }
            }

            OriginalPlayButton.Visible = false;
            OriginalPauseButton.Visible = false;
            OriginalFrameButton.Visible = false;

            PlayHostButton = (ToolStripButton)Editor.Instance.UI.ToolStrip.AddButton(PlayHost.FindSprite("Default"),
                OnClickedHost).LinkTooltip("Play as a Host");

            PlayClientButton = (ToolStripButton)Editor.Instance.UI.ToolStrip.AddButton(PlayClient.FindSprite("Default"),
                OnClickedClient).LinkTooltip("Play as a Client");

            Editor.Instance.UI.ToolStrip.PerformLayout(true);
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
