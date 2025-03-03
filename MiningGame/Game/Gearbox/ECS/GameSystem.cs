using System.Collections.Generic;
using System.Numerics;
using Engine;
using Physics;

namespace Engine
{
    public abstract class GameSystem
    {
        public List<Component> validComponents = new(); //Doesnt have a use right now
        public virtual void Start() { } //Start of system
        public virtual void Update(float delta) { } //Updates every frame
    }
    public interface IScript { } // Infterface for scriptes (has atomatic update update)
}