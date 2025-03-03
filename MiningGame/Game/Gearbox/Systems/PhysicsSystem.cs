using System.Numerics;
using Raylib_cs;
using CoreEngine;
using Engine;

namespace Physics
{
    public class PhysicsSystem : GameSystem //Handle physics
    {
        public override void Update(float delta)
        {
            foreach (GameEntity gameEntity in Core.activeGameEntities) //loop all entitys
            {
                PhysicsBody? physicsBody = gameEntity.GetComponent<PhysicsBody>(); //Look if it has physicsBody

                if (physicsBody != null) //check if it has one
                {
                    UpdatePhysics(physicsBody, delta);
                    Core.UpdateChildren(gameEntity.transform.parent);// update transform
                }
            }
        }
        //Handle acceleration, velocity, position and drag
        void UpdatePhysics(PhysicsBody pB, float delta)
        {
            if (pB.physicsType == PhysicsBody.PhysicsType.staticType) //use static for checks
            {
                return;
            }
            // Apply drag separately to X and Y
            pB.velocity.X *= 1 - pB.dragX * delta;
            pB.velocity.Y *= 1 - pB.dragY * delta;

            pB.acceleration += pB.Gravity * delta * 60; //calc gravity

            pB.velocity += pB.acceleration * delta; //calc velocity from acceleration

            pB.gameEntity.transform.position += pB.velocity * delta; // calc position from velocity

            pB.acceleration = Vector2.Zero; //Reset acceleration
        }
    }
}