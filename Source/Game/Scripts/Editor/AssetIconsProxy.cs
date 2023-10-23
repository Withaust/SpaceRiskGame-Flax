#if FLAX_EDITOR
using FlaxEditor;
using FlaxEditor.Content;
using FlaxEngine;
using Game;
using System;
using System.Collections.Generic;
using System.IO;

public class CustomJsonAssetItem : JsonAssetItem
{
    public CustomJsonAssetItem(string path, Guid id, string typeName)
    : base(path, id, typeName)
    {
        _thumbnail = AssetIconsProxy.Atlases[typeName].FindSprite("Default");
    }
}

public class JsonAssetProxy<T> : SpawnableJsonAssetProxy<T> where T : new()
{
    public override AssetItem ConstructItem(string path, string typeName, ref Guid id)
    {
        return new CustomJsonAssetItem(path, id, typeName);
    }
}

public class AssetIconsProxy
{
    public static Dictionary<string, SpriteAtlas> Atlases { get; private set; } = new Dictionary<string, SpriteAtlas>();
    private static List<JsonAssetProxy> Proxies = new List<JsonAssetProxy>();

    private static void RegAssetIcon<T>(string Path = "Document") where T : ISerializable, new()
    {
        Type type = typeof(T);
        string typeName = type.FullName;
        if (Atlases.ContainsKey(typeName))
        {
            return;
        }
        LoadIcon(Path, typeName);
        Proxies.Add(new JsonAssetProxy<T>());
    }

    private static void LoadIcon(string IconPath, string Key)
    {
        var iconsAtlas = Content.Load<SpriteAtlas>(Path.Combine(Globals.ProjectContentFolder, "Editor/Textures/Content/" + IconPath + ".flax"));
        if (iconsAtlas is null || iconsAtlas.WaitForLoaded())
        {
            return;
        }

        Atlases[Key] = iconsAtlas;
    }

    public static void ApplyProxies()
    {
        foreach (var Proxy in Proxies)
        {
            Editor.Instance.ContentDatabase.AddProxy(Proxy);
        }
        Editor.Instance.ContentDatabase.Rebuild();
    }

    public static void RemoveProxies()
    {
        foreach (var Proxy in Proxies)
        {
            Editor.Instance.ContentDatabase.RemoveProxy(Proxy);
        }
        Proxies.Clear();
        Editor.Instance.ContentDatabase.Rebuild();
    }

    public static void LoadIcons()
    {
        RegAssetIcon<DrillData>();
    }
}
#endif
