#if FLAX_EDITOR
using System;
using FlaxEngine;
using FlaxEditor;
using FlaxEditor.SceneGraph;

namespace Game
{
    public class EntityProxy : FlaxEngine.Object
    {
        public static void Add()
        {
            SceneGraphFactory.CustomNodesTypes.Add(typeof(Entity), typeof(EntityIconProxy));
        }
    }

    public sealed class EntityIconProxy : ActorNodeWithIcon
    {
        /// <inheritdoc />
        public EntityIconProxy(Actor actor)
            : base(actor)
        {
        }
    }
}
#endif
