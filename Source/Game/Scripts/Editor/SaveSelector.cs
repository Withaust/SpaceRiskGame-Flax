#if FLAX_EDITOR
using FlaxEditor;
using FlaxEngine.GUI;
using System.IO;

namespace Game
{
    public class SaveSelector
    {
        private static Dropdown _worldDropdown;
        private static Dropdown _characterDropdown;

        private static string _selectedWorld = string.Empty;
        private static string _selectedCharacter = string.Empty;

        private static string worldPath = "PersistentData/World";
        private static string characterPath = "PersistentData/Character";

        public static void Refresh()
        {
            if (!Directory.Exists("PersistentData"))
            {
                Directory.CreateDirectory("PersistentData");
            }

            if (!Directory.Exists(worldPath))
            {
                Directory.CreateDirectory(worldPath);
            }

            if (!Directory.Exists(characterPath))
            {
                Directory.CreateDirectory(characterPath);
            }

            if (!File.Exists(worldPath + "/Default.json"))
            {
                Saver.SaveDefaultWorld();
            }

            if (!File.Exists(characterPath + "/Default.json"))
            {
                Saver.SaveDefaultCharacter();
            }

            _worldDropdown.ClearItems();
            string[] worldFiles = Directory.GetFiles(worldPath, "*.*");
            foreach (string worldFile in worldFiles)
            {
                _worldDropdown.AddItem(Path.GetFileNameWithoutExtension(worldFile));
            }

            if(_selectedWorld != string.Empty && _worldDropdown.Items.Contains(_selectedWorld))
            {
                _worldDropdown.SelectedItem = _selectedWorld;
            }
            else
            {
                _worldDropdown.SelectedItem = "Default";
                _selectedWorld = "Default";
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
            label.Text = "World:";
            label.AutoWidth = true;
            _worldDropdown = Editor.Instance.UI.ToolStrip.AddChild<Dropdown>();
            _worldDropdown.SelectedIndexChanged += WorldChanged;
            label = Editor.Instance.UI.ToolStrip.AddChild<Label>();
            label.Text = "Character:";
            label.AutoWidth = true;
            _characterDropdown = Editor.Instance.UI.ToolStrip.AddChild<Dropdown>();
            _characterDropdown.SelectedIndexChanged += CharacterChanged;
            Refresh();
        }

        private static void CharacterChanged(Dropdown obj)
        {
            LaunchArgs.ForceCharacter = obj.SelectedItem;
        }

        private static void WorldChanged(Dropdown obj)
        {
            LaunchArgs.ForceWorld = obj.SelectedItem;
        }

        public static void RemoveButton()
        {
            if (_worldDropdown != null)
            {
                _worldDropdown.Dispose();
                _worldDropdown = null;
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
