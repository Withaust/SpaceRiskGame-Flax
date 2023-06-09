using System;
using FlaxEditor;
using FlaxEditor.Content;
using FlaxEngine;
using Game;

public class MySettingsItem : JsonAssetItem
{
    public MySettingsItem(string path, Guid id, string typeName)
    : base(path, id, typeName)
    {
        _thumbnail = Editor.Instance.Icons.Document128;
    }
}

[ContentContextMenu("New/EditorLaunchArgs")]
public class MySettingsProxy : SpawnableJsonAssetProxy<EditorLaunchArgs>
{
    public override AssetItem ConstructItem(string path, string typeName, ref Guid id)
    {
        return new MySettingsItem(path, id, typeName);
    }
}