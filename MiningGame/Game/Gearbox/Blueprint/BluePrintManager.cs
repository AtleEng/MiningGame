using System;
using System.Reflection;
using System.Text.Json;

namespace Engine
{
    public static class ECSFactory
    {
        public static GameEntity BuildEntity(EntityBlueprint blueprint)
        {
            // Create a new ECS entity.
            GameEntity entity = new GameEntity();
            // Optionally, you could add a “Name” component to store the entity name.

            foreach (var compBlueprint in blueprint.Components)
            {
                // Assume the component type is fully qualified. For example "Engine.Sprite"
                Type? compType = Type.GetType(compBlueprint.ComponentType);
                if (compType == null)
                {
                    Console.WriteLine($"Component type '{compBlueprint.ComponentType}' not found.");
                    return new GameEntity();
                }
                Component? component = Activator.CreateInstance(compType) as Component;

                // Set properties from the blueprint.
                foreach (var kvp in compBlueprint.Properties)
                {
                    // Try property first.
                    PropertyInfo? propInfo = compType.GetProperty(kvp.Key, BindingFlags.Public | BindingFlags.Instance);
                    if (propInfo != null && propInfo.CanWrite)
                    {
                        object value = ConvertJsonElement(kvp.Value, propInfo.PropertyType);
                        propInfo.SetValue(component, value);
                    }
                    else
                    {
                        // Then try public field.
                        FieldInfo fieldInfo = compType.GetField(kvp.Key, BindingFlags.Public | BindingFlags.Instance);
                        if (fieldInfo != null)
                        {
                            object value = ConvertJsonElement(kvp.Value, fieldInfo.FieldType);
                            fieldInfo.SetValue(component, value);
                        }
                        else
                        {
                            Console.WriteLine($"Property or field '{kvp.Key}' not found on {compType.Name}");
                        }
                    }
                }
                // Add the component to the entity.
                if (component != null)
                {
                    entity.AddComponent<Component>(component);
                }
            }
            foreach (EntityBlueprint childBlueprint in blueprint.Children)
            {
                GameEntity child = BuildEntity(childBlueprint);
                entity.transform.children.Add(child.transform);
            }
            return entity;
        }

        private static object ConvertJsonElement(JsonElement element, Type targetType)
        {
            // Basic type conversions. Extend these as needed.
            if (targetType == typeof(int))
                return element.GetInt32();
            if (targetType == typeof(float))
                return element.GetSingle();
            if (targetType == typeof(double))
                return element.GetDouble();
            if (targetType == typeof(bool))
                return element.GetBoolean();
            if (targetType == typeof(string))
                return element.GetString();
            if (targetType == typeof(System.Numerics.Vector2))
            {
                // Assume a JSON array with two elements: [x, y]
                if (element.ValueKind == JsonValueKind.Array && element.GetArrayLength() == 2)
                {
                    float x = element[0].GetSingle();
                    float y = element[1].GetSingle();
                    return new System.Numerics.Vector2(x, y);
                }
            }
            // Fallback: attempt to deserialize using the JSON serializer.
            return JsonSerializer.Deserialize(element.GetRawText(), targetType);
        }
        public static void SaveBlueprint(EntityBlueprint blueprint, string filePath)
        {
            var options = new JsonSerializerOptions { WriteIndented = true };
            string json = JsonSerializer.Serialize(blueprint, options);
            File.WriteAllText(filePath, json);
        }

        public static EntityBlueprint LoadBlueprint(string filePath)
        {
            string json = File.ReadAllText(filePath);
            return JsonSerializer.Deserialize<EntityBlueprint>(json);
        }
    }
}
