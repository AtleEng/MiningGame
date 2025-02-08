using System.Collections.Generic;
using System.Numerics;
using Engine;
using Physics;

namespace Engine
{
    //The class that handles components
    public abstract class Component
    {
        //the attached component's entity
        public Entity? entity;

        //When entity spawns
        public virtual void OnStart() { }
        //Every frame
        public virtual void OnUpdate(float delta) { }
        //Before entity is destroyed
        public virtual void OnDestroy() { }
        //If entity has a trigger collider and it is triggered
        public virtual void OnTrigger(Collider other) { }
        //If entity has a collider and it is colliding
        public virtual void OnCollision(Collider other) { }

        public virtual string PrintStats() { return ""; } //debug tree (press F3)
    }
}