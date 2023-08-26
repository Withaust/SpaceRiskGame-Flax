using System;
using System.Collections.Generic;
using FlaxEngine;
using FlaxEngine.Networking;

namespace Game
{
    public class TestDummy : IComponent
    {
        private float MaterialTimer = 0.0f;

        public IDamage Damage = null;
        public IHitBox Hitbox = null;
        public TextRender Text = null;
        public float Health = 10000.0f;
        public MaterialInstance DefaultMaterial = null;
        public MaterialInstance HitMaterial = null;
        public StaticModel Model = null;
        
        private void OnDamage(uint HitBox, Entity Inflictor, float Damage)
        {
            Hitbox.Health = Health;
            Text.Text = "HitBox: " + HitBox + "\n" + "Damager: " + Inflictor.Name + "\n" + "Damage: " + ((int)Damage).ToString();
            Model.SetMaterial(0, HitMaterial);
            MaterialTimer = 1.0f;
        }

        public override void OnEnable()
        {
            Damage.OnDamage += OnDamage;
            NetworkReplicator.AddObject(this);
        }

        public override void OnDisable()
        {
            Damage.OnDamage -= OnDamage;
        }

        public override void OnUpdate()
        {
            MaterialTimer -= Time.DeltaTime;
            if(MaterialTimer <= 0.0f ) 
            {
                Model.SetMaterial(0, DefaultMaterial);
                Text.Text = "0";
            }
        }
    }
}
