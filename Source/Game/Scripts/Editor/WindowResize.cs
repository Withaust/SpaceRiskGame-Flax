#if FLAX_EDITOR
using FlaxEditor;
using FlaxEngine;
using FlaxEngine.GUI;
using System;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

namespace Game
{
    public class WindowResize
    {
        public static void ProcessCmd()
        {
            var args = Environment.GetCommandLineArgs();
            for(int i = 0; i < args.Length; ++i) 
            {
                string arg = args[i];
                if(arg == "-size")
                {
                    string size = args[i + 1];
                    Regex sizeRegex = new("(.*[0-9])x(.*[0-9])");
                    MatchCollection matches = sizeRegex.Matches(size);
                    if(matches.Count == 1)
                    {
                        int x = int.Parse(matches[0].Groups[1].Value);
                        int y = int.Parse(matches[0].Groups[2].Value);
                        Editor.Instance.Windows.MainWindow.ClientSize = new Float2(x, y);
                    }
                }
                else if(arg == "-position")
                {
                    string position = args[i + 1];
                    Regex positionRegex = new("(.*[0-9])x(.*[0-9])");
                    MatchCollection matches = positionRegex.Matches(position);
                    if (matches.Count == 1)
                    {
                        int x = int.Parse(matches[0].Groups[1].Value);
                        int y = int.Parse(matches[0].Groups[2].Value);
                        Editor.Instance.Windows.MainWindow.Position = new Float2(x, y);
                    }
                }
            }
        }
    }
}
#endif
