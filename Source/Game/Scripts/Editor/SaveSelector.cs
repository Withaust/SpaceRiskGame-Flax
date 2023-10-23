#if FLAX_EDITOR
using FlaxEditor;
using FlaxEngine.GUI;
using System.IO;

namespace Game
{
    public class SaveSelector
    {
        private static Dropdown _universeDropdown;
        private static Dropdown _characterDropdown;

        private static string _selectedUniverse = string.Empty;
        private static string _selectedCharacter = string.Empty;

        private static string universePath = "PersistentData/Universe";
        private static string characterPath = "PersistentData/Character";

        public static void Refresh()
        {
            if (!Directory.Exists("PersistentData"))
            {
                Directory.CreateDirectory("PersistentData");
            }

            if (!Directory.Exists(universePath))
            {
                Directory.CreateDirectory(universePath);
            }

            if (!Directory.Exists(characterPath))
            {
                Directory.CreateDirectory(characterPath);
            }

            if (!File.Exists(universePath + "/Default.json"))
            {
                Saver.SaveDefaultUniverse();
            }

            if (!File.Exists(characterPath + "/Default.json"))
            {
                Saver.SaveDefaultCharacter();
            }

            _universeDropdown.ClearItems();
            string[] universeFiles = Directory.GetFiles(universePath, "*.*");
            foreach (string universeFile in universeFiles)
            {
                _universeDropdown.AddItem(Path.GetFileNameWithoutExtension(universeFile));
            }

            if (_selectedUniverse != string.Empty && _universeDropdown.Items.Contains(_selectedUniverse))
            {
                _universeDropdown.SelectedItem = _selectedUniverse;
            }
            else
            {
                _universeDropdown.SelectedItem = "Default";
                _selectedUniverse = "Default";
            }

            _characterDropdown.ClearItems();
            string[] characterFiles = Directory.GetFiles(characterPath, "*.*");
            foreach (string characterFile in characterFiles)
            {
                _characterDropdown.AddItem(Path.GetFileNameWithoutExtension(characterFile));
            }

            if (_selectedCharacter != string.Empty && _characterDropdown.Items.Contains(_selectedCharacter))
            {
                _characterDropdown.SelectedItem = _selectedCharacter;
            }
            else
            {
                _characterDropdown.SelectedItem = "Default";
                _selectedCharacter = "Default";
            }
        }

        public static void AddButton()
        {
            var label = Editor.Instance.UI.ToolStrip.AddChild<Label>();
            label.Text = "Universe:";
            label.AutoWidth = true;
            _universeDropdown = Editor.Instance.UI.ToolStrip.AddChild<Dropdown>();
            _universeDropdown.SelectedIndexChanged += UniverseChanged;
            label = Editor.Instance.UI.ToolStrip.AddChild<Label>();
            label.Text = "Character:";
            label.AutoWidth = true;
            _characterDropdown = Editor.Instance.UI.ToolStrip.AddChild<Dropdown>();
            _characterDropdown.SelectedIndexChanged += CharacterChanged;
            Refresh();
        }

        private static void CharacterChanged(Dropdown obj)
        {
            LaunchArgs.Character = obj.SelectedItem;
        }

        private static void UniverseChanged(Dropdown obj)
        {
            LaunchArgs.Universe = obj.SelectedItem;
        }

        public static void RemoveButton()
        {
            if (_universeDropdown != null)
            {
                _universeDropdown.Dispose();
                _universeDropdown = null;
            }
            if (_characterDropdown != null)
            {
                _characterDropdown.Dispose();
                _characterDropdown = null;
            }
        }

    }
}
#endif
