using System.Numerics;

namespace Engine
{
    //Transform is the class that handle position, size and parent system
    public class UITransform : TransformBase
    {
        public UITransform(TransformBase parent, List<TransformBase> children, Vector2 position, Vector2 size, Vector2 anchor)
        {
            this.parent = parent;
            this.children = children;
            this.position = position;
            this.size = size;
            this.anchor = anchor;
        }
        public Vector2 anchor = new Vector2(0.5f,0.5f);
    }
}