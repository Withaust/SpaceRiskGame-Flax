#if FLAX_EDITOR
using FlaxEditor.CustomEditors;
using FlaxEditor.CustomEditors.Editors;
using FlaxEditor.CustomEditors.Elements;
using FlaxEngine;

namespace Game
{
    [CustomEditor(typeof(WorldSave))]
    public class WorldSaveEditor : GenericEditor
    {
        TextBoxElement _worldInput = null;
        ButtonElement _worldButton = null;

        private void OnWorldSave()
        {
            if (Saver.Instance == null)
            {
                return;
            }

            Saver.Instance.SaveWorldAs(_worldInput.Text);
        }

        public override void Initialize(LayoutElementsContainer layout)
        {
            base.Initialize(layout);

            layout.Label("World Name", TextAlignment.Center);
            _worldInput = layout.TextBox();
            _worldInput.Text = "Default";
            _worldButton = layout.Button("Save World", Color.Green);
            _worldButton.Button.Clicked += OnWorldSave;
        }
    }

    [CustomEditor(typeof(CharacterSave))]
    public class CharacterSaveEditor : GenericEditor
    {
        TextBoxElement _characterInput = null;
        ButtonElement _characterButton = null;

        private void OnCharacterSave()
        {
            if (Saver.Instance == null)
            {
                return;
            }

            Saver.Instance.SaveCharacterAs(_characterInput.Text);
        }

        public override void Initialize(LayoutElementsContainer layout)
        {
            base.Initialize(layout);

            layout.Label("Character Name", TextAlignment.Center);
            _characterInput = layout.TextBox();
            _characterInput.Text = "Default";
            _characterButton = layout.Button("Save Character", Color.Green);
            _characterButton.Button.Clicked += OnCharacterSave;
        }
    }
}
#endif
