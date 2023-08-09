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
    [CustomEditor(typeof(PlayerManager))]
    public class PlayerManagerCSV : CSVEditor<PlayerManager>
    {
        public override void OnInit()
        {
            AddBinding("List", "PlayerId", true, () =>
            {
                foreach (var Player in Get.GetPlayers())
                {
                    Set(Player.Key, "Name", Player.Value.GetComponent<PawnInfo>().NameLocal);
                    Set(Player.Key, "Position", Player.Value.GetComponent<PlayerMovement>().Controller.Position);
                }
            });
        }
    }

    [CustomEditor(typeof(Entity))]
    public class EntityCSV : CSVEditor<Entity>
    {
        public override void OnInit()
        {
            AddBinding("Component Ids", "ComponentId", true, () =>
            {
                foreach (var Component in Get.GetComponents())
                {
                    Set(Component.Key, "Id", Component.Value.ID);
                }
            });
        }
    }
}
#endif
