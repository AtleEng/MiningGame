using Physics;

namespace Engine
{
    public class Check : GameEntity
    {
        //Add all diffrent components
        public Check(int colliderLayer)
        {
            name = "Check";

            PhysicsBody physicsBody = new()
            {
                physicsType = PhysicsBody.PhysicsType.staticType
            };
            AddComponent<PhysicsBody>(physicsBody);
            Collider collider = new
            (
                true, colliderLayer
            );
            AddComponent<Collider>(collider);
        }
    }
}