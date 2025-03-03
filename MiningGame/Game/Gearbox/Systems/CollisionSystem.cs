using System.Numerics;
using Raylib_cs;
using CoreEngine;
using Engine;

namespace Physics
{
    //system that handle collision
    public class CollisionSystem : GameSystem
    {
        public override void Update(float delta)
        {
            //* set all colliders isColliding to false 
            foreach (GameEntity gameEntity in Core.activeGameEntities)//loop thruogh all active entitys
            {
                Collider? collider = gameEntity.GetComponent<Collider>(); //Check if entity has a collider
                if (collider != null)
                {
                    collider.isColliding = false; //set the isCollideing to false (if it collides this loop it will be turned to true)
                }
            }
            //*Detect and solve collisions
            foreach (GameEntity gameEntity in Core.activeGameEntities)//loop thruogh all active entitys
            {
                PhysicsBody? physicsBody = gameEntity.GetComponent<PhysicsBody>();//Get physicsBody
                Collider? collider = gameEntity.GetComponent<Collider>(); //Get collider

                if (collider != null && physicsBody != null) //if entity has both collider and physicsbody
                {
                    List<Collider> colliders = DetectCollision(collider, physicsBody); //Get all colliders that collide with this collider
                    //if isnt trigger solve the collision
                    if (!collider.isTrigger)
                    {
                        //Solve the collision between all the colliders that collide with this collider
                        SolveCollision(colliders, physicsBody, collider);
                        //Update transform
                        Core.UpdateChildren(physicsBody.gameEntity.transform.parent);
                    }
                }
            }
        }
        //Detect collisions and call OnTrigger (if collider = isTrigger)
        List<Collider> DetectCollision(Collider collider, PhysicsBody physicsBody)
        {
            List<Tuple<Collider, float>> collidersAndArea = new(); //List with turple that link collider with collision area

            Rectangle aabb = GetRectangleFromCollider(collider); //gets world collision rect

            foreach (GameEntity otherGameEntity in Core.activeGameEntities) //loop all entitys
            {
                if (otherGameEntity != collider.gameEntity) //if not the same entity
                {
                    Collider? otherCollider = otherGameEntity.GetComponent<Collider>(); //Get its collider
                    if (otherCollider != null)
                    {
                        if (PhysicsSettings.collisionMatrix[collider.layer, otherCollider.layer]) //Check if colliders can collide (physicsSettings to change interactions)
                        {
                            Rectangle otherAabb = GetRectangleFromCollider(otherCollider); //Get aabb from the collider components

                            Rectangle collisonRec = Raylib.GetCollisionRec(aabb, otherAabb); //Get the overlap rect from both colliders
                            float area = collisonRec.Width * collisonRec.Height; //Get overlap area
                            if (area != 0) //Check so they collide
                            {
                                //Set colliders to isColliding for script logic
                                collider.isColliding = true;
                                otherCollider.isColliding = true;
                                //If a collider is trigger call OnTrigger else and if it has physics OnCollision
                                if (collider.isTrigger || otherCollider.isTrigger)
                                {
                                    collider.gameEntity.OnTrigger(otherCollider);
                                }
                                else if (physicsBody != null)
                                {
                                    collider.gameEntity.OnCollision(otherCollider);
                                    collidersAndArea.Add(new(otherCollider, area)); //Add in turple list for the solver to use
                                }
                            }
                        }
                    }
                }
            }
            return SortOtherAabbsByArea(collidersAndArea); //Sort collider by how big their overlap is (biggest first)
        }
        //* Solve the collision for entitys with physicsbodys and isTrigger = false
        void SolveCollision(List<Collider> other, PhysicsBody physicsBody, Collider collider)
        {
            for (int i = 0; i < other.Count; i++) // loop all collision of entity
            {
                Rectangle aabb = GetRectangleFromCollider(collider);//Get aabb from the collider components
                Rectangle otherAabb = GetRectangleFromCollider(other[i]);//Get aabb from the other collider components

                Rectangle collisonRec = Raylib.GetCollisionRec(aabb, otherAabb); //Get the overlap rect from both colliders

                //* separate diffrent collision (if width is bigger or smaller the height)
                if (collisonRec.Width < collisonRec.Height && collisonRec.Width * collisonRec.Height != 0)
                {
                    int dir = Math.Sign(physicsBody.velocity.X); //find dir
                    if (aabb.X < otherAabb.X) //If aabb is right of otherAabb
                    {
                        collider.gameEntity.transform.position.X = otherAabb.X - aabb.Width / 2; //move aabb to correct position
                        if (dir > 0)
                        {
                            physicsBody.velocity.X *= -dir * physicsBody.elasticity; //change velocity
                        }
                    }
                    else //aabb is left of otherAabb (the same as above but left)
                    {
                        collider.gameEntity.transform.position.X = otherAabb.X + otherAabb.Width + aabb.Width / 2;
                        if (dir < 0)
                        {
                            physicsBody.velocity.X *= dir * physicsBody.elasticity;
                        }
                    }
                }
                else if (collisonRec.Width * collisonRec.Height != 0) //The same as above but for y collisions
                {
                    int dir = Math.Sign(physicsBody.velocity.Y);
                    if (aabb.Y < otherAabb.Y)
                    {
                        collider.gameEntity.transform.position.Y = otherAabb.Y - aabb.Height / 2;
                        if (dir > 0)
                        {
                            physicsBody.velocity.Y *= -dir * physicsBody.elasticity;
                        }
                    }
                    else
                    {
                        collider.gameEntity.transform.position.Y = otherAabb.Y + otherAabb.Height + aabb.Height / 2;
                        if (dir < 0)
                        {
                            physicsBody.velocity.Y *= dir * physicsBody.elasticity;
                        }
                    }
                }
                Core.UpdateChildren(collider.gameEntity.transform.parent); //update transform
            }
        }
        Rectangle GetRectangleFromCollider(Collider collider) //Method that return world rect from collider component
        {
            return new Rectangle
            (
                collider.gameEntity.transform.worldPosition.X + collider.offset.X - collider.gameEntity.transform.worldSize.X * collider.scale.X / 2,
                collider.gameEntity.transform.worldPosition.Y + collider.offset.Y - collider.gameEntity.transform.worldSize.Y * collider.scale.Y / 2,
                collider.gameEntity.transform.worldSize.X * collider.scale.X,
                collider.gameEntity.transform.worldSize.Y * collider.scale.Y
            );
        }
        List<Collider> SortOtherAabbsByArea(List<Tuple<Collider, float>> other) // sort colliders by its collision overlap area 
        {
            // Sort the tuples by area in descending order
            var sortedTuples = other.OrderByDescending(t => t.Item2);

            // Make a list of the colliders from the sorted tuples
            List<Collider> sortedColliders = sortedTuples.Select(t => t.Item1).ToList();

            return sortedColliders;
        }
        public static class PhysicsSettings //controlls some physics interations
        {
            public static bool[,] collisionMatrix = new bool[4, 4]
            {
            //true = collide / false = ignore collision
            //player ground check enemy
            { false, true, false, true}, //player
            { true, true, true, true}, //ground
            { false, true, false, false}, //check
            { false, true, false, false} //enemy
            };
        }
    }
}