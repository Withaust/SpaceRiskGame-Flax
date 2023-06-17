#if FLAX_EDITOR
using FlaxEditor;
using FlaxEditor.Content;
using FlaxEditor.GUI;
using FlaxEngine;

namespace Game
{
    public class EditorPlugin : FlaxEditor.EditorPlugin
    {
        public override void InitializeEditor()
        {
            base.InitializeEditor();
            AssetIconsProxy.LoadIcons();
            AssetIconsProxy.ApplyProxies();
            CustomPlayButtons.Editor = Editor;
            CustomPlayButtons.AddCustomPlayButtons();
        }

        public override void DeinitializeEditor()
        {
            AssetIconsProxy.RemoveProxies();
            CustomPlayButtons.RemoveCustomPlayButtons();
            base.DeinitializeEditor();
        }
    }
}

#endif