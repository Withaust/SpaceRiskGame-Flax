#if FLAX_EDITOR
using FlaxEditor;

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
            TerrainDataAssetProxy.ApplyProxies();
            CustomPlayButtons.AddCustomPlayButtons();
            SaveSelector.AddButton();
            UIRefresh.AddButton();
            CSGRefresh.AddButton();
            WindowResize.ProcessCmd();

            Editor.UI.ToolStrip.PerformLayout(true);
        }

        public override void DeinitializeEditor()
        {
            TerrainDataAssetProxy.RemoveProxies();
            AssetIconsProxy.RemoveProxies();
            SaveSelector.RemoveButton();
            UIRefresh.RemoveButton();
            CSGRefresh.RemoveButton();
            CustomPlayButtons.RemoveCustomPlayButtons();
            Editor.Instance.StateMachine.PlayingState.GameSettingsApplied -= OnStartedPlaying;
            Editor.Instance.StateMachine.PlayingState.SceneRestored -= OnStoppedPlaying;
            base.DeinitializeEditor();
        }
    }
}

#endif