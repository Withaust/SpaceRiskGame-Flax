using System;
using System.Collections.Generic;
using FlaxEngine;

namespace Game
{
    public class TestTrigger : Script
    {
        public FilterTrigger FilterTrigger { get; set; }
        public override void OnStart()
        {
            FilterTrigger.OnTriggerEnter += OnTriggerEnter;
            FilterTrigger.OnTriggerLeave += OnTriggerLeave;
        }

        private void OnTriggerEnter(Entity obj)
        {
            Logger.Instance.Info("Entred by " + obj.Name);
        }

        private void OnTriggerLeave(Entity obj)
        {
            Logger.Instance.Info("Left by " + obj.Name);
        }
    }
}
