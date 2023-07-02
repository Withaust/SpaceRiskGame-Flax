using System;
using System.Collections.Generic;
using FlaxEngine;
using FlaxEngine.Networking;

namespace Game
{
    public class TestBush : Script
    {
        public BoxCollider Collider;

        private void Collider_TriggerEnter(PhysicsColliderActor obj)
        {
            Chat.Instance.SendMessage("HELLO");
        }

        private void Collider_TriggerExit(PhysicsColliderActor obj)
        {
            Chat.Instance.SendMessage("BYE BYE");
        }

        public override void OnEnable()
        {
            if (NetworkManager.IsHost)
            {
                Collider.TriggerEnter += Collider_TriggerEnter;
                Collider.TriggerExit += Collider_TriggerExit;
            }
        }

        public override void OnDisable()
        {
            if (NetworkManager.IsHost)
            {
                Collider.TriggerEnter -= Collider_TriggerEnter;
                Collider.TriggerExit -= Collider_TriggerExit;
            }
        }
    }
}
