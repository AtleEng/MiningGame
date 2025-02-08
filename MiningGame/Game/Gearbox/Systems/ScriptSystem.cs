using System.Numerics;

using CoreEngine;
using Engine;

namespace CoreEngine
{
    //Handle scripts logic
    public class ScriptSystem : GameSystem
    {
        //Give all IScript a proper start function
        public override void OnSystemStart()
        {
            foreach (GameEntity gameEntity in Core.activeGameEntities)
            {
                foreach (Component component in gameEntity.components)
                {
                    if (component is IScript)
                    {
                        component.OnStart();
                    }
                }
            }
        }
        //Give all IScript a proper update function
        public override void OnSystemUpdate(float delta)
        {
            foreach (GameEntity gameEntity in Core.gameEntities)
            {
                foreach (Component component in gameEntity.components)
                {
                    if (component is IScript)
                    {
                        component.OnUpdate(delta);
                    }
                }
            }
        }
    }
}