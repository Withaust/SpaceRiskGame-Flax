#if FLAX_EDITOR
using FlaxEditor;
using FlaxEditor.GUI;
using FlaxEngine;
using Newtonsoft.Json.Linq;
using System.Collections.Generic;
using System.IO;
using ClosedXML.Excel;
using System;

namespace Game
{
    public class XLSXExport
    {
        private static SpriteAtlas _buttonAtlas;
        private static ToolStripButton _button;
        private static string _folderDir = Path.Combine(Globals.ProjectFolder, "XLSX");
        private static string _dataDir = Path.Combine(Globals.ProjectFolder, "Content", "Data");

        private static void ProcessEntries(string Prefix, JProperty property, ref Dictionary<string, string> output, ref Dictionary<string, ParsedObject> guidObjects)
        {
            if (property.Type == JTokenType.Object)
            {
                JObject data = property.Value<JObject>();
                foreach (var prop in data.Properties())
                {
                    ProcessEntries(Prefix + "." + property.Name, prop, ref output, ref guidObjects);
                }
            }
            else
            {
                Guid guid;
                try
                {
                    guid = Guid.Parse(property.Value.ToString());
                }
                catch (Exception)
                {
                    output[Prefix + "." + property.Name] = property.Value.ToString();
                    return;
                }

                if (guid == null)
                {
                    output[Prefix + "." + property.Name] = property.Value.ToString();
                    return;
                }
                string guidStr = guid.ToString().Replace("-", "");
                if (!guidObjects.ContainsKey(guidStr))
                {
                    output[Prefix + "." + property.Name] = property.Value.ToString();
                    return;
                }
                ParsedObject target = guidObjects[guidStr];
                output[Prefix + "." + property.Name] = "=HYPERLINK(\"[" + target.FileName + "]" + target.WorkSheet + "!A" + target.Row + ":ZZ" + target.Row + "\",\"REF\")";
            }
        }

        private struct ParsedObject
        {
            public JObject Data;
            public string FileName;
            public string WorkSheet;
            public int Row;
        }

        private static void Export()
        {
            if (Directory.Exists(_folderDir))
            {
                foreach (string FilePath in Directory.GetFiles(_folderDir))
                {
                    string FileName = Path.GetFileNameWithoutExtension(FilePath);
                    if (!FileName.StartsWith("~$"))
                    {
                        continue;
                    }
                    MessageBox.Show("It seems that Excel is currently running, and holding file "
                        + FileName.Replace("~$", "") + ".xlsx from deleting.\nClose Excel and try again.");
                    return;
                }
                Directory.Delete(_folderDir, true);
            }
            Directory.CreateDirectory(_folderDir);

            string[] directories = Directory.GetDirectories(_dataDir);

            Dictionary<string, ParsedObject> guidObjects = new Dictionary<string, ParsedObject>();
            Dictionary<string, Dictionary<string, ParsedObject>> parsed = new Dictionary<string, Dictionary<string, ParsedObject>>();

            foreach (string directory in directories)
            {
                string baseClassName = Path.GetFileName(directory);

                int CurrentRow = 1;

                foreach (string file in Directory.GetFiles(directory))
                {
                    string className = Path.GetFileNameWithoutExtension(file);
                    JObject json = JObject.Parse(File.ReadAllText(file));
                    string id = json.GetValue("ID").Value<string>();
                    JObject data = json.GetValue("Data").Value<JObject>();
                    ParsedObject parsedObject = new()
                    {
                        Data = data,
                        FileName = baseClassName + ".xlsx",
                        WorkSheet = baseClassName,
                        Row = CurrentRow
                    };
                    guidObjects[id] = parsedObject;

                    if (!parsed.ContainsKey(baseClassName))
                    {
                        parsed[baseClassName] = new Dictionary<string, ParsedObject>();
                    }

                    parsed[baseClassName][className] = parsedObject;
                    CurrentRow++;
                }
            }

            foreach (var baseClass in parsed)
            {
                Dictionary<string, Dictionary<string, string>> Properties = new Dictionary<string, Dictionary<string, string>>();

                foreach (var targetClass in baseClass.Value)
                {
                    Dictionary<string, string> result = new Dictionary<string, string>();
                    foreach (var property in targetClass.Value.Data.Properties())
                    {
                        ProcessEntries(baseClass.Key, property, ref result, ref guidObjects);
                    }
                    Properties[targetClass.Key] = result;
                }

                var workbook = new XLWorkbook();
                var worksheet = workbook.Worksheets.Add(baseClass.Key);

                worksheet.Cell(1, 1).Value = "        Class Name        ";
                int rowCounter = 2;
                int columnCounter = 2;
                int columnLength = 0;
                foreach (var property in Properties)
                {
                    worksheet.Cell(rowCounter, 1).FormulaA1 = "=HYPERLINK(\"..\\Content\\Data\\" + baseClass.Key + "\",\"" + property.Key + "\")";
                    worksheet.Cell(rowCounter, 1).Style.Font.FontColor = XLColor.Blue;
                    foreach (var properties in property.Value)
                    {
                        string targetValue = properties.Value;
                        float numberic;
                        if (float.TryParse(targetValue, out numberic))
                        {
                            worksheet.Cell(rowCounter, columnCounter).Value = numberic;
                        }
                        else
                        {
                            if (targetValue.StartsWith("=HYPERLINK"))
                            {
                                worksheet.Cell(rowCounter, columnCounter).FormulaA1 = targetValue;
                                worksheet.Cell(rowCounter, columnCounter).Style.Font.FontColor = XLColor.Blue;
                            }
                            else
                            {
                                worksheet.Cell(rowCounter, columnCounter).Value = targetValue;
                            }
                        }
                        worksheet.Cell(1, columnCounter).Value = properties.Key;
                        columnCounter++;
                    }
                    if (columnCounter > columnLength)
                    {
                        columnLength = columnCounter;
                    }
                    columnCounter = 2;
                    rowCounter++;
                }

                var content = worksheet.Columns(1, columnLength);
                content.Style.Alignment.Horizontal = XLAlignmentHorizontalValues.Center;
                content.Style.Alignment.Vertical = XLAlignmentVerticalValues.Center;
                content.Style.Font.FontName = "Arial";
                content.Style.Font.FontSize = 14;

                content.AdjustToContents();

                workbook.SaveAs(Path.Combine(_folderDir, baseClass.Key + ".xlsx"));
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
