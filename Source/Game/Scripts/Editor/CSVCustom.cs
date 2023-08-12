#if FLAX_EDITOR
using FlaxEditor.CustomEditors;
using FlaxEngine;

namespace Game
{
    [CustomEditor(typeof(PlayerManager))]
    public class PlayerManagerCSV : CSVScript<PlayerManager>
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
    public class EntityCSV : CSVActor<Entity>
    {
        public override void OnInit()
        {
            AddBinding("Components", "ComponentId", true, () =>
            {
                foreach (var Component in Get.GetComponents())
                {
                    Set(Component.Key, "Guid", Component.Value.ID);
                }
            });
        }
    }
}
#endif
