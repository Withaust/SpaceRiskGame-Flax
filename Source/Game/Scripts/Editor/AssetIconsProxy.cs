using System;
using System.IO;
using System.Collections.Generic;
using FlaxEditor;
using FlaxEditor.Content;
using FlaxEngine;
using Game;
using System.Threading.Tasks;
using System.Threading;

public class AssetIconsProxy
{
    public static Dictionary<string, SpriteAtlas> Atlases { get; private set; } = new Dictionary<string, SpriteAtlas>();

    private static void ParseEntry(ContentItem Entry)
    {
        if (Entry.ItemType == ContentItemType.Folder && Entry is ContentFolder Folder)
        {
            foreach (var Child in Folder.Children)
            {
                ParseEntry(Child);
            }
            return;
        }

        if (Entry.SearchFilter != ContentItemSearchFilter.Json)
        {
            return;
        }

        AssetInfo Info;
        Content.GetAssetInfo(Entry.Path, out Info);
        string Type = Info.TypeName;

        if (Atlases.ContainsKey(Type))
        {
            Entry.Thumbnail = Atlases[Type].FindSprite("Default");
            return;
        }

        if(Type.StartsWith("Game."))
        {
            Entry.Thumbnail = Atlases["Game.Default"].FindSprite("Default");
        }
    }

    public static void OnContentItem(ContentItem Item)
    {
        Task.Factory.StartNew(() => Thread.Sleep(1000)).ContinueWith((t) => ParseEntry(Editor.Instance.ContentDatabase.Game.Content.Folder));
    }

    public static void ApplyIcons()
    {
        Editor.Instance.ContentDatabase.ItemAdded += OnContentItem;
        Editor.Instance.ContentDatabase.ItemRemoved += OnContentItem;
        LoadIcon("Document", "Game.Default");
        LoadIcons();
        ParseEntry(Editor.Instance.ContentDatabase.Game.Content.Folder);
    }

    private static void AddCustomIcon<T>(string Path) where T : ISerializable, new()
    {
        Type type = typeof(T);
        string typeName = type.FullName;
        if (Atlases.ContainsKey(typeName))
        {
            return;
        }
        LoadIcon(Path, typeName);
    }

    private static void LoadIcon(string IconPath, string Key)
    {
        var iconsAtlas = Content.Load<SpriteAtlas>(System.IO.Path.Combine(Globals.ProjectContentFolder, "Editor/Textures/Content/" + IconPath + ".flax"));
        if (iconsAtlas is null || iconsAtlas.WaitForLoaded())
        {
            return;
        }

        Atlases[Key] = iconsAtlas;
    }

    private static void LoadIcons()
    {
        //AddCustomIcon<EditorLaunchArgs>("Document");
    }
}
