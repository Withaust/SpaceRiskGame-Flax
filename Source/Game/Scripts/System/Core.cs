using FlaxEngine;

namespace Game
{
    partial class Core : Actor
    {
        public static T Get<T>() where T : ISystem
        {
            return (T)Get(typeof(T));
        }
    }
}
