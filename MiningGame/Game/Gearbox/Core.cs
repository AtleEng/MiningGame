using System.Collections.Generic;
using System.Numerics;
using System.IO;
using Raylib_cs;

using Engine;
using Animation;
using Physics;
using Graphics;


namespace CoreEngine
{
    //The core of the game, handles systems
    public static class Core
    {
        //All entitys in scene in child of this
        public static GameEntity rotEntity = new GameEntity();
        public static bool shouldClose; //If true the program closes

        static public List<GameEntity> gameEntities = new(); //All gameEntitys
        static public List<GameEntity> activeGameEntities = new(); //All active gameEntitys

        static public List<GameSystem> systems = new(); //All diffent systems

        static public List<GameEntity> entitiesToAdd = new(); //All entitys to add after update
        static public List<GameEntity> entitiesToRemove = new(); //All entitys to remove after update

        //deltaTime variables
        static float oldTime = 0;
        static float newTime = 0;

        public static void Start()
        {
            //Add all systems in update order
            AddSystem<ScriptSystem>();
            AddSystem<PhysicsSystem>();
            AddSystem<CollisionSystem>();
            AddSystem<AnimationSystem>();
            AddSystem<RenderSystem>();

            //* Innit all the systems in the right order
            systems[systems.Count - 1].Start(); //Start the render system first
            for (int i = 0; i < systems.Count - 1; i++)
            {
                systems[i].Start();
            }
            //Audio
            Raylib.InitAudioDevice();
            Raylib.SetMasterVolume(0.5f);

            rotEntity.OnInnit();

            //* Update all systems in the right order
            while (shouldClose == false)
            {
                //Clac delta time
                oldTime = newTime;
                newTime = (float)Raylib.GetTime();
                float deltaTime = newTime - oldTime;

                Update(deltaTime);
            }
        }
        static void Update(float delta)
        {
            activeGameEntities.Clear(); //Reset it

            GetAllActiveEntities(rotEntity); //Refill the active entity list

            // Uppdate all the systems in the right order
            // TODO nu hoppar den över physics och collision om delta är för hög inte jättebra lösning men fungerar
            for (int i = 0; i < systems.Count; i++)
            {
                if (!(delta > 0.02f && (i == 1 || i == 2)))
                {
                    systems[i].Update(delta);
                }
            }
            UpdateChildren(rotEntity.transform); //Update all transforms
            // Add and remove games entities
            foreach (var entity in entitiesToAdd)
            {
                gameEntities.Add(entity);
            }
            foreach (var entity in entitiesToRemove)
            {
                if (entity.transform.parent != null)
                {
                    entity.transform.parent.children.Remove(entity.transform);
                }
                gameEntities.Remove(entity);
            }
            //clear the lists
            entitiesToAdd.Clear();
            entitiesToRemove.Clear();
            //Prints out a tree of the entitys relations and components in the console
            if (Raylib.IsKeyPressed(KeyboardKey.F3))
            {
                //Console.Clear();
                PrintEntityTree(rotEntity, "", "");
            }
        }
        static public void UpdateChildren(Engine.Transform parent) //Updates the child transform to move and scale with the parent
        {
            foreach (Engine.Transform child in parent.children)
            {
                child.worldPosition = child.position + parent.worldPosition;
                child.worldSize = child.size * parent.worldSize;

                UpdateChildren(child);
            }
        }
        public static void AddSystem<T>() where T : GameSystem //Uses reflection to create a new system from type
        {
            GameSystem system = (GameSystem)Activator.CreateInstance(typeof(T));
            systems.Add(system);
        }
        public static void RemoveSystem<T>() where T : GameSystem //Remove a system of a type
        {
            foreach (GameSystem gS in systems)
            {
                if (gS.GetType() == typeof(T))
                {
                    systems.Remove(gS);
                }
            }
        }

        //* For debugging
        static void PrintEntityTree(GameEntity entity, string layer = "", string space = "")
        {
            Console.WriteLine($"{space}{layer}{entity.name} [{entity.PrintStats()}]");

            // Components
            if (entity.components.Count > 0)
            {
                foreach (Component component in entity.components)
                {
                    Console.WriteLine($"| {space}+{component.GetType().Name} [{component.PrintStats()}]");
                }
            }
            // Entities
            if (entity.transform.children.Count > 0)
            {
                foreach (var child in entity.transform.children)
                {
                    PrintEntityTree(child.gameEntity, $"{layer}>", $"{space}| "); //reapetes until all children of scene are printed
                }
            }
            System.Console.WriteLine($"{space}<");
        }
        static void GetAllActiveEntities(GameEntity entity) //Get all entitys in gameEntitys with isActive == true
        {
            if (entity.transform.children.Count > 0)
            {
                foreach (Engine.Transform child in entity.transform.children)
                {
                    if (child.gameEntity.isActive)
                    {
                        activeGameEntities.Add(child.gameEntity);
                        GetAllActiveEntities(child.gameEntity);
                    }
                }

            }
        }
        static public void ChangeRotEntity(GameEntity gameEntity)
        {
            EntityManager.DestroyEntity(rotEntity);
            rotEntity = gameEntity;
            rotEntity.OnInnit();
        }
        static public void QuitGame()
        {
            shouldClose = true;
        }
    }
}