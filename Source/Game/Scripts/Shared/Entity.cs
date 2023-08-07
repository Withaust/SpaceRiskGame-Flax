using System;
using System.Collections.Generic;
using FlaxEngine;

namespace Game
{
    partial class Entity : Actor
    {
        public T GetComponent<T>() where T : IComponent
        {
            return (T)GetComponent(typeof(T));
        }
    }
}
