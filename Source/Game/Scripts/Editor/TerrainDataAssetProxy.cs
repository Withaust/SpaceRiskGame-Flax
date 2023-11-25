#if FLAX_EDITOR
using System;
using FlaxEditor;
using FlaxEditor.Content;
using FlaxEditor.Windows;
using FlaxEngine;

namespace Game
{
    public class TerrainDataAssetProxy : BinaryAssetProxy
    {
        private static TerrainDataAssetProxy _instance = null;
        public static TerrainDataAssetProxy Instance
        {
            get
            {
                if (_instance == null)
                {
                    _instance = new TerrainDataAssetProxy();
                }
                return _instance;
            }
        }

        public static void ApplyProxies()
        {
            Editor.Instance.ContentDatabase.AddProxy(Instance);
            Editor.Instance.ContentDatabase.Rebuild();
        }

        public static void RemoveProxies()
        {
            Editor.Instance.ContentDatabase.RemoveProxy(Instance);
            Editor.Instance.ContentDatabase.Rebuild();
        }

        /// <inheritdoc />
        public override string Name => "TerrainDataAsset";

        /// <inheritdoc />
        public override EditorWindow Open(Editor editor, ContentItem item)
        {
            return null;
        }

        /// <inheritdoc />
        public override Color AccentColor => Color.FromRGB(0xccff33);

        /// <inheritdoc />
        public override Type AssetType => typeof(TerrainDataAsset);

        /// <inheritdoc />
        public override bool CanCreate(ContentFolder targetLocation)
        {
            return false;
        }

        /// <inheritdoc />
        public override void Create(string outputPath, object arg)
        {
        }
    }
}
#endif
