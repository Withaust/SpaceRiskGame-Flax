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
            bool HadPosition = false;

            bool HadPopout = false;
            Float2 PopoutSize = new Float2(200.0f, 200.0f);
            Float2 PopoutPos = new Float2();

            var args = Environment.GetCommandLineArgs();
            for(int i = 0; i < args.Length; ++i) 
            {
                string arg = args[i];
                if (arg == "-popout")
                {
                    HadPopout = true;
                }
                else if (arg == "-size")
                {
                    string size = args[i + 1];
                    Regex sizeRegex = new("(.*[0-9])x(.*[0-9])");
                    MatchCollection matches = sizeRegex.Matches(size);
                    if(matches.Count == 1)
                    {
                        int x = int.Parse(matches[0].Groups[1].Value);
                        int y = int.Parse(matches[0].Groups[2].Value);
                        if(HadPopout)
                        {
                            PopoutSize = new Float2(x, y);
                        }
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
                        if (HadPopout)
                        {
                            PopoutPos = new Float2(x, y);
                        }
                        Editor.Instance.Windows.MainWindow.Position = new Float2(x, y);
                        HadPosition = true;
                    }
                }
            }

            if(HadPopout)
            {
                Editor.Instance.Windows.GameWin.ShowFloating(PopoutPos, PopoutSize);
            }
            else if(!HadPosition)
            {
                Editor.Instance.Windows.MainWindow.Position = new Float2(0.0f, 0.0f);
            }
        }
    }
}
#endif
