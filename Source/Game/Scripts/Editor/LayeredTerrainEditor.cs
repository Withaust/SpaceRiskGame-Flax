#if FLAX_EDITOR
using FlaxEditor;
using FlaxEditor.Content;
using FlaxEditor.CustomEditors;
using FlaxEditor.CustomEditors.Editors;
using FlaxEditor.Windows;
using FlaxEngine;
using System;

namespace Game
{
    [CustomEditor(typeof(LayeredTerrain))]
    public class LayeredTerrainEditor : GenericEditor
    {
        private void OnClicked()
        {
            if (IsSingleObject)
            {
                var target = Values[0] as LayeredTerrain;
                target.RebuildData();
            }
        }

        public override void Initialize(LayoutElementsContainer layout)
        {
            base.Initialize(layout);

            layout.Space(20);
            var button = layout.Button("Rebuild Data", Color.Green);
            button.Button.Clicked += OnClicked;
        }
    }
}
#endif
