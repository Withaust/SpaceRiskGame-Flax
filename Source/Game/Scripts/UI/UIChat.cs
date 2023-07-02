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
            NewEntry.Text = PlayerManager.Instance.GetPlayerInfo(Sender).Name + ": " + Message;
            Messages.Enqueue(NewEntry);
        }

        public void Focus()
        {
            IsFocused = true;
            TextBox.Control.Focus();
            PlayerManager.Instance.GetOurPlayer().GetScript<PlayerMovement>().CanMove = false;
        }

        public void OnSendMessage(KeyboardKeys keys)
        {
            if(keys != KeyboardKeys.Escape && keys != KeyboardKeys.Return)
            {
                return;
            }
            Chat.Instance.SendMessage(TextBox.Get<TextBox>().Text);
            TextBox.Control.Defocus();
            TextBox.Get<TextBox>().Text = "";
            PlayerManager.Instance.GetOurPlayer().GetScript<PlayerMovement>().CanMove = true;
            IsFocused = false;
        }

        public override void OnStart()
        {
            Messages = new Queue<Label>();
            Chat.Instance.OnChatMessage += OnChatMessage;
            TextBox.Get<TextBox>().KeyDown += OnSendMessage;
        }
    }
}
