#if FLAX_EDITOR
using FlaxEditor;
using FlaxEditor.Content;
using FlaxEditor.GUI;
using FlaxEngine;
using System.IO;
using System.Linq;

namespace Game
{
    public class UIRefresh
    {
        private static SpriteAtlas _buttonAtlas;
        private static ToolStripButton _button;
        private static string _inputDir = Path.Combine(Globals.ProjectFolder, "UI", "Data");
        private static string _outputDir = Path.Combine(Globals.ProjectFolder, "Content", "UI");

        private static void OnClicked()
        {
            var rmlFiles = Directory.GetFiles(_inputDir, "*.rml", SearchOption.AllDirectories);
            var files = rmlFiles.Concat(Directory.GetFiles(_inputDir, "*.rcss", SearchOption.AllDirectories));

            foreach (var file in files)
            {
                string targetDir = file.Replace(_inputDir, "");
                targetDir = _outputDir + Path.GetDirectoryName(targetDir);
                targetDir = Path.TrimEndingDirectorySeparator(targetDir);
                if (!Directory.Exists(targetDir))
                {
                    Directory.CreateDirectory(targetDir);
                    Editor.Instance.ContentDatabase.Rebuild(true);
                }
                Editor.Instance.ContentImporting.Import(file, (ContentFolder)Editor.Instance.ContentDatabase.Find(targetDir), true);
            }

            UI.RefreshCache();
        }

        public static void AddButton()
        {
            _buttonAtlas = Content.Load<SpriteAtlas>(Path.Combine(Globals.ProjectContentFolder, "Editor/Textures/UI/UI.flax"));
            _button = Editor.Instance.UI.ToolStrip.AddButton(_buttonAtlas.FindSprite("Default"));
            _button.Clicked += OnClicked;
        }

        public static void RemoveButton()
        {
            if (_button != null)
            {
                _button.Dispose();
                _button = null;
            }
        }

    };
}
#endif
