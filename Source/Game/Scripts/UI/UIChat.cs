using System;
using System.Collections.Generic;
using FlaxEngine;
using FlaxEngine.GUI;
using FlaxEngine.Networking;

namespace Game
{
    public class UIChat : Script
    {
        public bool IsFocused = false;
        public UIControl Root;
        public UIControl TextBox;
        public Prefab Entry;
        private Queue<Label> Messages;

        public void OnChatMessage(uint Sender, string Message)
        {
            if(Messages.Count > 8)
            {
                Label control = Messages.Dequeue();
                control.Dispose();
            }
            Label NewEntry = PrefabManager.SpawnPrefab(Entry, Root).As<UIControl>().Get<Label>();
            NewEntry.Text = Core.Get<PlayerManager>().GetPlayerInfo(Sender).Name + ": " + Message;
            Messages.Enqueue(NewEntry);
        }

        public void Focus()
        {
            IsFocused = true;
            TextBox.Control.Focus();
            Core.Get<PlayerManager>().GetOurPlayer().GetScript<PlayerMovement>().CanMove = false;
        }

        public void OnSendMessage(KeyboardKeys keys)
        {
            if(keys != KeyboardKeys.Escape && keys != KeyboardKeys.Return)
            {
                return;
            }
            Core.Get<Chat>().SendMessage(new NetworkRpcParams(), TextBox.Get<TextBox>().Text);
            TextBox.Control.Defocus();
            TextBox.Get<TextBox>().Text = "";
            Core.Get<PlayerManager>().GetOurPlayer().GetScript<PlayerMovement>().CanMove = true;
            IsFocused = false;
        }

        public override void OnStart()
        {
            Messages = new Queue<Label>();
            Core.Get<Chat>().OnChatMessage += OnChatMessage;
            TextBox.Get<TextBox>().KeyDown += OnSendMessage;
        }
    }
}
