#if FLAX_EDITOR
using FlaxEditor;
using FlaxEditor.GUI;
using FlaxEngine;

namespace Game
{
    public class EditorPlugin : FlaxEditor.EditorPlugin
    {
        public override void InitializeEditor()
        {
            base.InitializeEditor();
            Editor.Instance.ContentDatabase.Proxy.Add(new MySettingsProxy());
        }

        /// <inheritdoc />
        public override void Deinitialize()
        {
            Editor.Instance.ContentDatabase.Proxy.RemoveAll(proxy => proxy is MySettingsProxy);
            base.Deinitialize();
        }
    }
}

#endif