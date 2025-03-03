using System.Numerics;
using Engine;

namespace Physics
{
    //Component used to add physics to gameEntity, also makes it possible to interact by collision
    public class PhysicsBody : Component
    {
        //dynamic => moving objects, static => triggers (walls and still blocks dosnt need a physicsBody)
        public PhysicsType physicsType = PhysicsType.dynamicType;

        //How much force impact object (Dosnt work with collision right now)
        public float mass = 1;

        //Speed and direction of the body
        public Vector2 velocity = Vector2.Zero;
        //How much the speed increses
        public Vector2 acceleration = Vector2.Zero;

        //how much the body is slowed down
        public float dragX = 0f;
        public float dragY = 0f;

        //How much velocity is retain per bounce
        public float elasticity = 0f;
        //Acceleration in a direction (Default mimics earths)
        public Vector2 Gravity = new Vector2(0, 9.82f);

        //add a force to entity
        public void AddForce(Vector2 force)
        {
            if (mass == 0) { mass = 1; }

            acceleration += force / mass;
        }
        public override string PrintStats()//for debug parent tree (press F3)
        {
            return $"Velocity: {velocity}, Mass: {mass} Drag: {dragX}, {dragY}, Elasticity: {elasticity}";
        }

        public enum PhysicsType
        {
            dynamicType, staticType
        }
    }
}