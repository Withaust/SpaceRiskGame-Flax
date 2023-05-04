using System;
using System.Collections.Generic;
using FlaxEngine;

namespace Game
{
    public class UIGame : IUIState
    {
        public UIChat Chat;

        public override void OnInitialize()
        {
           
        }

        public override void OnDeinitialize()
        {
            
        }

        public override void OnEnable()
        {
            Screen.CursorLock = CursorLockMode.Locked;
            Screen.CursorVisible = false;
        }

        public override void OnUpdate()
        {
            if(Input.GetKeyUp(KeyboardKeys.T) && !Chat.IsFocused)
            {
                Chat.Focus();
            }
        }

        public override void OnDisable()
        {
            Screen.CursorLock = CursorLockMode.None;
            Screen.CursorVisible = true;
        }

        public override string GetBackState()
        {
            return "None";
        }
    }
}
