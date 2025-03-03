using System.Collections.Generic;
using System.Numerics;
using Physics;

namespace Engine
{
    //Transform is the class that handle position, size and parent system
    public class Transform
    {
        //Constructor
        public Transform(Transform parent, List<Transform> children, Vector2 position, Vector2 size)
        {
            this.parent = parent;
            this.children = children;
            this.position = position;
            this.size = size;
        }
        //The transforms entity
        public GameEntity gameEntity;
    
        public Vector2 position;
        public Vector2 worldPosition = Vector2.Zero;

        public Vector2 size;
        public Vector2 worldSize = Vector2.One;

        public Transform? parent;
        public List<Transform> children = new();
    }
}