using System.Numerics;

namespace Engine
{
    public abstract class TransformBase
    {
        public GameEntity gameEntity;
        public Vector2 position;
        public Vector2 size;

        public TransformBase? parent;
        public List<TransformBase> children = new();
    }
    //Transform is the class that handle position, size and parent system
    public class Transform : TransformBase
    {
        public Transform(TransformBase parent, List<TransformBase> children, Vector2 position, Vector2 size)
        {
            this.parent = parent;
            this.children = children;
            this.position = position;
            this.size = size;
        }
        public Vector2 worldPosition = Vector2.Zero;
        public Vector2 worldSize = Vector2.One;
    }
}