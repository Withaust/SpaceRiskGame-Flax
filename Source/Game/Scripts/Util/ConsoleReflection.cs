using System;
using System.Collections.Generic;
using System.Reflection;
using FlaxEngine;

namespace Game
{
    [Serializable]
    [AttributeUsage(AttributeTargets.Method | AttributeTargets.Property | AttributeTargets.Field)]
    public sealed class CmdAttribute : Attribute
    {
        public CmdAttribute()
        {
        }
    }

    class ConsoleReflection
    {
        private enum EntryType
        {
            Field,
            Property,
            Method
        }

        private struct Entry
        {
            public EntryType Type;
            public Script Target;
            public FieldInfo Field;
            public PropertyInfo Property;
            public MethodInfo Method;
        }

        static Dictionary<string, Entry> Entries = new Dictionary<string, Entry>();

        static public void BuildSuggestions()
        {
            Scene targetScene = Level.Scenes[0];
            if (targetScene.Name != "Core")
            {
                Logger.Instance.Error("Somehow Level.Scenes[0] is not a Core scene. This should not happen");
                return;
            }

            // Core (Scene)
            // L_ Core (Game.Core)
            //    L_ Core (Actor)
            //    |  L_ Systems (ISystem)
            //    |  L_ ...
            //    L_ Game (Actor)
            //    |  L_ Systems (ISystem)
            //    |  L_ ...

            Core Core = (Core)targetScene.Children[0];

            Actor CoreRoot = Core.Children[0];
            Actor GameRoot = Core.Children[1];

            List<Script> Systems = new List<Script>();

            for (int i = 0; i < CoreRoot.Children.Length; i++)
            {
                Script instance = CoreRoot.Children[i].Scripts[0];
                Systems.Add(instance);
            }

            for (int i = 0; i < GameRoot.Children.Length; i++)
            {
                Script instance = GameRoot.Children[i].Scripts[0];
                Systems.Add(instance);
            }

            foreach (var System in Systems)
            {
                Type type = System.GetType();
                var Fields = type.GetFields(BindingFlags.Instance | BindingFlags.Public);
                foreach (var Field in Fields)
                {
                    foreach (var Attribute in Field.CustomAttributes)
                    {
                        if (Attribute.AttributeType == typeof(CmdAttribute))
                        {
                            Entries[type.Name.Replace("Game.", "") + "." + Field.Name] = new Entry()
                            {
                                Target = System,
                                Type = EntryType.Field,
                                Field = Field
                            };
                        }
                    }
                }
                var Properties = type.GetProperties(BindingFlags.Instance | BindingFlags.Public);
                foreach (var Property in Properties)
                {
                    foreach (var Attribute in Property.CustomAttributes)
                    {
                        if (Attribute.AttributeType == typeof(CmdAttribute))
                        {
                            Entries[type.Name.Replace("Game.", "") + "." + Property.Name] = new Entry()
                            {
                                Target = System,
                                Type = EntryType.Property,
                                Property = Property
                            };
                        }
                    }
                }
                var Methods = type.GetMethods(BindingFlags.Instance | BindingFlags.Public);
                foreach (var Method in Methods)
                {
                    foreach (var Attribute in Method.CustomAttributes)
                    {
                        if (Attribute.AttributeType == typeof(CmdAttribute))
                        {
                            Entries[type.Name.Replace("Game.", "") + "." + Method.Name] = new Entry()
                            {
                                Target = System,
                                Type = EntryType.Method,
                                Method = Method
                            };
                        }
                    }
                }
            }
        }

        static public List<string> GetSuggestions(string Text, int Limit = 5)
        {
            List<string> result = new List<string>();
            foreach (var Entry in Entries)
            {
                string FilteredText = Text.Split(' ')[0].Split('(')[0];
                if(!Entry.Key.StartsWith(FilteredText))
                {
                    continue;
                }

                Entry Value = Entry.Value;
                if(Value.Type == EntryType.Field)
                {
                    object Target = Value.Field.GetValue(Value.Target);
                    result.Add(Entry.Key + " = " + Target.ToString());
                }
                else if(Value.Type == EntryType.Property)
                {
                    object Target = Value.Property.GetValue(Value.Target);
                    result.Add(Entry.Key + " = " + Target.ToString());
                }
                // Method
                else
                {
                    string Arguments = default;
                    var Parameters = Value.Method.GetParameters();
                    for(int i = 0; i < Parameters.Length; i++)
                    {
                        var Parameter = Parameters[i];
                        Arguments += Parameter.ParameterType.Name + " " + Parameter.Name;
                        if(i != Parameters.Length - 1)
                        {
                            Arguments += ", ";
                        }
                    }
                    result.Add(Entry.Key + "(" + Arguments + ")");
                }

                if(result.Count >= Limit)
                {
                    break;
                }
            }
            return result;
        }
    }
}
