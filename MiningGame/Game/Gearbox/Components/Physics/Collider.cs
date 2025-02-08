using System.Numerics;
using Engine;

namespace Physics
{
    //Component used to collide with other colliders
    public class Collider : Component
    {
        //Offset from gameEntity
        public Vector2 offset;
        //Size scale
        public Vector2 scale = Vector2.One;
        //If collider is TRIGGER it dosnt collide but read if something enter its area
        public bool isTrigger;
        //Change which colliders collide in physics setting
        public int layer;

        public bool isColliding;
        public Collider(bool isTrigger, int layer)
        {
            this.isTrigger = isTrigger;
            this.layer = layer;
        }
        public override string PrintStats()//for debug parent tree (press F3)
        {
            return $"Trigger: {isTrigger}, Layer: {layer}";
        }
    }

}