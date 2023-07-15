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
            XLSXExport.AddButton(Editor);
            UIRefresh.AddButton(Editor);
        }

        public override void DeinitializeEditor()
        {
            AssetIconsProxy.RemoveProxies();
            UIRefresh.RemoveButton();
            XLSXExport.RemoveButton();
            CustomPlayButtons.RemoveCustomPlayButtons();
            base.DeinitializeEditor();
        }
    }
}

#endif