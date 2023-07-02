#if FLAX_EDITOR
using FlaxEditor;
using FlaxEditor.GUI;
using FlaxEditor.States;
using FlaxEngine;
using FlaxEngine.GUI;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using ClosedXML.Excel;
using System.Diagnostics;

namespace Game
{
    public class XLSXExport
    {
        private static SpriteAtlas _buttonAtlas;
        private static ToolStripButton _button;
        private static string _folderDir = Path.Combine(Globals.ProjectFolder, "XLSX");
        private static string _dataDir = Path.Combine(Globals.ProjectFolder, "Content", "Data");

        private static void Export()
        {
            if(Directory.Exists(_folderDir))
            {
                Directory.Delete(_folderDir, true);
            }
            Directory.CreateDirectory(_folderDir);

            string[] directories = Directory.GetDirectories(_dataDir);
            foreach (string directory in directories)
            {
                Dictionary<string, Dictionary<string, string>> Properties = new Dictionary<string, Dictionary<string, string>>();
                
                foreach (string file in Directory.GetFiles(directory))
                {
                    string className = Path.GetFileNameWithoutExtension(file);
                    JObject json = JObject.Parse(File.ReadAllText(file));
                    string id = json.GetValue("ID").Value<string>();
                    JObject data = json.GetValue("Data").Value<JObject>();
                    Properties[className] = new Dictionary<string, string>();
                    foreach (var property in data.Properties())
                    {
                        Properties[className][property.Name] = property.Value.ToString();
                    }
                }

                string baseClassName = Path.GetFileName(directory);

                var workbook = new XLWorkbook();
                var worksheet = workbook.Worksheets.Add(baseClassName);

                var First = Properties.First();

                int columnStart = 2;
                int columnCounter = columnStart;
                worksheet.Cell(1, 1).Value = "Class";
                foreach (var property in First.Value)
                {
                    worksheet.Cell(1, columnCounter).Value = property.Key;
                    columnCounter++;
                }
                int columnLength = columnCounter;
                columnCounter = columnStart;
                int rowCounter = 2;
                foreach (var property in Properties)
                {
                    worksheet.Cell(rowCounter, 1).Value = property.Key;
                    foreach (var properties in property.Value)
                    {
                        string targetValue = properties.Value;
                        float numberic;
                        if(float.TryParse(targetValue, out numberic))
                        {
                            worksheet.Cell(rowCounter, columnCounter).Value = numberic;
                        }
                        else
                        {
                            worksheet.Cell(rowCounter, columnCounter).Value = targetValue;
                        }
                        columnCounter++;
                    }
                    columnCounter = columnStart;
                    rowCounter++;
                }

                var content = worksheet.Range(1, 1, columnLength, rowCounter);
                content.Style.Alignment.Horizontal = XLAlignmentHorizontalValues.Center;
                content.Style.Alignment.Vertical = XLAlignmentVerticalValues.Center;
                content.Style.Font.FontName = "Arial";
                content.Style.Font.FontSize = 14;

                worksheet.Columns(1, columnLength).AdjustToContents();

                workbook.SaveAs(Path.Combine(_folderDir, baseClassName + ".xlsx"));
            }

            var settings = new CreateProcessSettings()
            {
                FileName = _folderDir,
                ShellExecute = true,
            };
            Platform.CreateProcess(ref settings);
        }

        private static void OnClicked()
        {
            Export();
        }

        public static void AddButton(Editor Editor)
        {
            _buttonAtlas = Content.Load<SpriteAtlas>(Path.Combine(Globals.ProjectContentFolder, "Editor/Textures/UI/XLSX.flax"));
            _button = Editor.UI.ToolStrip.AddButton(_buttonAtlas.FindSprite("Default"));
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
