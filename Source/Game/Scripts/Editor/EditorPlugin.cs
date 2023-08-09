#if FLAX_EDITOR
using FlaxEditor;
using FlaxEditor.Content;
using FlaxEditor.GUI;
using FlaxEngine;

namespace Game
{
    public class EditorPlugin : FlaxEditor.EditorPlugin
    {
        public static bool IsPlaying { get; private set; } = false;
        private static void OnStartedPlaying()
        {
            IsPlaying = true;
        }
        private void OnStoppedPlaying()
        {
            IsPlaying = false;
        }

        public override void InitializeEditor()
        {
            base.InitializeEditor();
            Editor.Instance.StateMachine.PlayingState.GameSettingsApplied += OnStartedPlaying;
            Editor.Instance.StateMachine.PlayingState.SceneRestored += OnStoppedPlaying;
            AssetIconsProxy.LoadIcons();
            AssetIconsProxy.ApplyProxies();
            CustomPlayButtons.AddCustomPlayButtons();
            SaveSelector.AddButton();
            UIRefresh.AddButton();
            Editor.UI.ToolStrip.PerformLayout(true);
        }

        public override void DeinitializeEditor()
        {
            AssetIconsProxy.RemoveProxies();
            SaveSelector.RemoveButton();
            UIRefresh.RemoveButton();
            CustomPlayButtons.RemoveCustomPlayButtons();
            Editor.Instance.StateMachine.PlayingState.GameSettingsApplied -= OnStartedPlaying;
            Editor.Instance.StateMachine.PlayingState.SceneRestored -= OnStoppedPlaying;
            base.DeinitializeEditor();
        }
    }
}

#endif