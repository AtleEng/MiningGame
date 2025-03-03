using System.Collections.Generic;
using System.Numerics;
using System.Text.Json;

namespace Engine
{
    public class ComponentBlueprint
    {
        // The fully qualified component type name ("Engine.Sprite")
        public string ComponentType { get; set; }

        // A dictionary of properties to set on the component.
        public Dictionary<string, JsonElement> Properties { get; set; } = new Dictionary<string, JsonElement>();
    }

    public class EntityBlueprint
    {
        public string name { get; set; }
        // Transform data for the entity.
        public Vector2 position;
        public Vector2 size;
        public float rotation;
        // List of component blueprints for this entity.
        public List<ComponentBlueprint> Components { get; set; } = new List<ComponentBlueprint>();
        // Optional child entities.
        public List<EntityBlueprint> Children { get; set; } = new List<EntityBlueprint>();
    }
}
