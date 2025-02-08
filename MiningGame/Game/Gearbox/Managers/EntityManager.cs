using System.Collections.Generic;
using System.Numerics;
using CoreEngine;
using Engine;

namespace Engine
{
    //handles all entities for the components to use
    public static class EntityManager
    {
        //Spaw in a entity in the world
        public static void SpawnEntity(GameEntity entity) { SpawnEntity(entity, Vector2.Zero); }
        public static void SpawnEntity(GameEntity entity, Vector2 position) { SpawnEntity(entity, position, Vector2.One); }
        public static void SpawnEntity(GameEntity entity, Vector2 position, Vector2 size) { SpawnEntity(entity, position, size, null); }
        public static void SpawnEntity(GameEntity entity, Vector2 position, Vector2 size, Transform parent)
        {
            entity.transform.position = position;
            entity.transform.size = size;

            entity.transform.gameEntity = entity;

            if (parent != null)
            {
                entity.transform.parent = parent;
            }
            else
            {
                entity.transform.parent = Core.rotEntity.transform;
            }
            entity.transform.parent.children.Add(entity.transform);

            entity.OnInnit();
            foreach (Component component in entity.components)
            {
                component.OnStart();
            }
            Core.entitiesToAdd.Add(entity);
        }
        //The way to remove entitys
        public static void DestroyEntity(GameEntity entity)
        {
            foreach (Component component in entity.components) //Call OnDestroy to all components
            {
                component.OnDestroy();
            }
            foreach (Transform child in entity.transform.children) //Destroy all children of entity
            {
                DestroyEntity(child.gameEntity);
            }
            Core.entitiesToRemove.Add(entity); //Finally remove entity
        }

        static public T? GetGameEntity<T>() where T : GameEntity //Gets first entity of matching type
        {
            foreach (GameEntity g in Core.gameEntities)
            {
                if (g.GetType() == typeof(T))
                {
                    return g as T;
                }
            }
            return null;
        }
    }
}