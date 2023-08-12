#if FLAX_EDITOR
using FlaxEditor.CustomEditors;
using FlaxEditor.CustomEditors.Dedicated;
using FlaxEditor.CustomEditors.Editors;
using FlaxEngine;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Game
{
    public class CSVExporter<T> where T : class
    {
        private struct Binding
        {
            public string KeyName;
            public Action Action;
            public bool RuntimeOnly;
        }

        private Dictionary<string, Binding> _bindings;
        private Dictionary<string, Dictionary<string, string>> _properties;

        public T Get
        {
            get => editor.Values[0] as T;
            set => Get = value;
        }

        public void Set(object Entry, object Name, object Value)
        {
            string EntryString = Entry.ToString();
            string NameString = Name.ToString();
            string ValueString = Value.ToString();

            if (!_properties.ContainsKey(EntryString))
            {
                _properties[EntryString] = new Dictionary<string, string>();
            }

            _properties[EntryString][NameString] = ValueString;
        }

        private void OnClickSave(string BindingName)
        {
            if (!editor.IsSingleObject)
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

        public void AddBinding(string BindingName, string KeyName, bool RuntimeOnly, Action Action)
        {
            _bindings[BindingName] = new Binding()
            {
                KeyName = KeyName,
                Action = Action,
                RuntimeOnly = RuntimeOnly
            };
        }

        private CustomEditor editor;

        public void Initialize(CustomEditor customEditor, Action onInit, LayoutElementsContainer layout)
        {
            editor = customEditor;
            _bindings = new Dictionary<string, Binding>();
            _properties = new Dictionary<string, Dictionary<string, string>>();
            onInit();
            foreach (var binding in _bindings)
            {
                var _button = layout.Button("CSV: " + binding.Key, Color.DarkSlateGray);
                _button.Button.Clicked += () => OnClickSave(binding.Key);
                _button.Button.Enabled = binding.Value.RuntimeOnly ? EditorPlugin.IsPlaying : true;
            }
        }
    }

    public abstract class CSVScript<T> : GenericEditor where T : class
    {
        static CSVExporter<T> exporter;

        public abstract void OnInit();
        public void AddBinding(string BindingName, string KeyName, bool RuntimeOnly, Action Action) { exporter.AddBinding(BindingName, KeyName, RuntimeOnly, Action); }
        public void Set(object Entry, object Name, object Value) { exporter.Set(Entry, Name, Value); }
        public T Get { get => exporter.Get; }

        public override void Initialize(LayoutElementsContainer layout)
        {
            base.Initialize(layout);
            exporter ??= new CSVExporter<T>();
            exporter.Initialize(this, OnInit, layout);
        }
    }

    public abstract class CSVActor<T> : ActorEditor where T : class
    {
        static CSVExporter<T> exporter;

        public abstract void OnInit();
        public void AddBinding(string BindingName, string KeyName, bool RuntimeOnly, Action Action) { exporter.AddBinding(BindingName, KeyName, RuntimeOnly, Action); }
        public void Set(object Entry, object Name, object Value) { exporter.Set(Entry, Name, Value); }
        public T Get { get => exporter.Get; }

        public override void Initialize(LayoutElementsContainer layout)
        {
            base.Initialize(layout);
            exporter ??= new CSVExporter<T>();
            exporter.Initialize(this, OnInit, layout);
        }
    }
}
#endif
