#if FLAX_EDITOR
using FlaxEditor.SceneGraph;
using FlaxEngine;

namespace Game
{
    public class EntityProxy : Object
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
