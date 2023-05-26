using FlaxEngine;
#if FLAX_EDITOR
using System;
using FlaxEditor;
using FlaxEditor.SceneGraph;
#endif
namespace Game
{
#if FLAX_EDITOR
    [ActorContextMenu("New/ActorBase")]
    [ActorToolbox("Other")]
#endif
    public class ActorBase : Actor
    {
#if FLAX_EDITOR
        private Guid _added;

        public Texture Icon;

        public float PropertyTest { get; set; }

        static ActorBase()
        {
            SceneGraphFactory.CustomNodesTypes.Add(typeof(ActorBase), typeof(ActorIconProxy));
        }

        public override void OnEnable()
        {
            base.OnEnable();
            if (Icon)
            {
                ViewportIconsRenderer.AddActorWithTexture(this, Icon);
                _added = Icon.ID;
            }
        }

        public override void OnDisable()
        {
            if (_added != Guid.Empty)
            {
                ViewportIconsRenderer.RemoveActor(this);
            }
            base.OnDisable();
        }

        public override void OnDebugDrawSelected()
        {
            base.OnDebugDrawSelected();
            if (Icon.ID != _added)
            {
                ViewportIconsRenderer.RemoveActor(this);
                ViewportIconsRenderer.AddActorWithTexture(this, Icon);
                _added = Icon.ID;
            }
        }
#endif
    }
}

#if FLAX_EDITOR
public sealed class ActorIconProxy : ActorNodeWithIcon
{
    /// <inheritdoc />
    public ActorIconProxy(Actor actor)
        : base(actor)
    {
    }
}
#endif
