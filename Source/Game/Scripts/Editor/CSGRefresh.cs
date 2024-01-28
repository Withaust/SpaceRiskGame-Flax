#if FLAX_EDITOR
using FlaxEditor;
using FlaxEditor.Content;
using FlaxEditor.GUI;
using FlaxEngine;
using System;
using System.IO;
using System.Linq;
using System.Threading;

namespace Game
{
    public class CSGRefresh
    {
        private static SpriteAtlas _buttonAtlas;
        private static ToolStripButton _button;

        private static void OnClicked()
        {
            Scene target = Level.GetScene(0);
            if (target == null) 
            {
                return;
            }

            StaticModel model = target.FindActor<StaticModel>("CSG");

            if(model == null)
            {
                return;
            }

            if (model.Model == null)
            {
                return;
            }

            model.Model.Reimport();

            MeshCollider collider = model.FindActor<MeshCollider>();
            if (collider == null) 
            { 
                return; 
            }

            if (collider.CollisionData == null)
            {
                return;
            }

            JobSystem.Dispatch(i => 
            {
                //model.Model.WaitForLoaded();
                Thread.Sleep(500);
                Editor.CookMeshCollision(collider.CollisionData.Path, CollisionDataType.TriangleMesh, model.Model);
            });
        }

        public static void AddButton()
        {
            _buttonAtlas = Content.Load<SpriteAtlas>(Path.Combine(Globals.ProjectContentFolder, "Editor/Textures/UI/CSG.flax"));
            _button = Editor.Instance.UI.ToolStrip.AddButton(_buttonAtlas.FindSprite("Default"));
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
