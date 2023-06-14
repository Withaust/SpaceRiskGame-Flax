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
            AssetIconsProxy.ApplyIcons();
        }

        /// <inheritdoc />
        public override void Deinitialize()
        {
            base.Deinitialize();
        }
    }
}

#endif