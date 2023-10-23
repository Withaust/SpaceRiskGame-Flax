#if FLAX_EDITOR
using FlaxEditor.CustomEditors;
using FlaxEditor.CustomEditors.Editors;
using FlaxEditor.CustomEditors.Elements;
using FlaxEngine;

namespace Game
{
    [CustomEditor(typeof(UniverseSave))]
    public class UniverseSaveEditor : GenericEditor
    {
        TextBoxElement _universeInput = null;
        ButtonElement _universeButton = null;

        private void OnUniverseSave()
        {
            if (Saver.Instance == null)
            {
                return;
            }

            Saver.Instance.SaveUniverseAs(_universeInput.Text);
        }

        public override void Initialize(LayoutElementsContainer layout)
        {
            base.Initialize(layout);

            layout.Label("Universe Name", TextAlignment.Center);
            _universeInput = layout.TextBox();
            _universeInput.Text = "Default";
            _universeButton = layout.Button("Save Universe", Color.Green);
            _universeButton.Button.Clicked += OnUniverseSave;
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
