#if FLAX_EDITOR
using FlaxEditor;
using FlaxEditor.CustomEditors.Editors;
using FlaxEditor.CustomEditors.Elements;
using FlaxEditor.CustomEditors;
using FlaxEngine;
using System.Collections.Generic;
using System;
using System.Linq;

namespace Game
{
    public abstract class CSVEditor<T> : GenericEditor where T : class
    {
        public abstract void OnInit();

        private struct Binding
        {
            public string KeyName;
            public Action Action;
        }

        private Dictionary<string, Binding> _bindings;
        private Dictionary<string, Dictionary<string, string>> _properties;

        public T Get
        {
            get => Values[0] as T;
            set => Get = value;
        }

        public void Set(object Entry, object Name, object Value)
        {
            string EntryString = Entry.ToString();
            string NameString = Name.ToString();
            string ValueString = Value.ToString();

            if(!_properties.ContainsKey(EntryString))
            {
                _properties[EntryString] = new Dictionary<string, string>();
            }

            _properties[EntryString][NameString] = ValueString;
        }

        private void OnClickSave(string BindingName)
        {
            if(!IsSingleObject)
            {
                return;
            }
            _properties.Clear();
            _bindings[BindingName].Action();

            if (_properties.Count == 0)
            {
                return;
            }

            var First = _properties.First();

            string result = _bindings[BindingName].KeyName;

            foreach (var Entry in First.Value)
            {
                result += ";" + Entry.Key;
            }

            result += Environment.NewLine;

            foreach (var Entry in _properties)
            {
                result += Entry.Key;
                foreach (var Values in Entry.Value)
                {
                    result += ";" + Values.Value;
                }
                result += Environment.NewLine;
            }

            Clipboard.Text = result;
        }

        public void AddBinding(string BindingName, string KeyName, Action Action)
        {
            _bindings[BindingName] = new Binding()
            {
                KeyName = KeyName,
                Action = Action
            };
        }

        public override void Initialize(LayoutElementsContainer layout)
        {
            _bindings = new Dictionary<string, Binding>();
            _properties = new Dictionary<string, Dictionary<string, string>>();
            OnInit();
            base.Initialize(layout);
            foreach (var binding in _bindings)
            {
                var _button = layout.Button("CSV: " + binding.Key, Color.DarkSlateGray);
                _button.Button.Clicked += () => OnClickSave(binding.Key);
            }
        }
    }

    [CustomEditor(typeof(PlayerManager))]
    public class PlayerManagerCSV : CSVEditor<PlayerManager>
    {
        public override void OnInit()
        {
            AddBinding("List", "PlayerId", () =>
            {
                foreach(var Player in Get.GetPlayers())
                {
                    Set(Player.Key, "Name", Player.Value.GetComponent<PawnInfo>().NameLocal);
                    Set(Player.Key, "Position", Player.Value.GetComponent<PlayerMovement>().Controller.Position);
                }
            });
        }
    }
}
#endif
