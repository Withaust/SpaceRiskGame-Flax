using System;
using System.Collections.Generic;
using FlaxEngine;
using FlaxEngine.Networking;

namespace Game
{
    public class TestDrill : Script
    {
        public BoxCollider Collider;
        public Drill Drill;

        [AssetReference(typeof(DrillData))]
        public JsonAsset Asset;

        private void Collider_TriggerEnter(PhysicsColliderActor obj)
        {
            if (!NetworkReplicator.IsObjectOwned(Drill))
            {
                return;
            }
            Drill.Data = Asset;
        }

        public override void OnEnable()
        {
            Collider.TriggerEnter += Collider_TriggerEnter;
        }

        public override void OnDisable()
        {
            Collider.TriggerEnter -= Collider_TriggerEnter;
        }
    }
}
