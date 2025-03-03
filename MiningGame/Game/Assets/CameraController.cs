using System.Numerics;
using System.Collections.Generic;
using Raylib_cs;

namespace Engine
{
    public class CameraController : Component, IScript
    {
        public CameraController(Transform player)
        {
            this.player = player;
        }
        Transform player;
        Vector2 offset = new(0, 0); //From the player
        float smoothing = 5; //how fast it will move towards the player
        public override void Update(float delta)
        {
            //Gets the camera bounds
           // float minX = WindowSettings.gameScreenWidth / (Camera.zoom * WorldSpace.pixelsPerUnit * 2);
            //float minY = WindowSettings.gameScreenHeight / (Camera.zoom * WorldSpace.pixelsPerUnit * 2);

            //float maxX = LoadingManager.LevelSize.X - 1 - WindowSettings.gameScreenWidth / (Camera.zoom * WorldSpace.pixelsPerUnit * 2);
            //float maxY = LoadingManager.LevelSize.Y - 1 - WindowSettings.gameScreenHeight / (Camera.zoom * WorldSpace.pixelsPerUnit * 2);

            //Lerp towards player
            Camera.position.X = Raymath.Lerp(Camera.position.X, player.worldPosition.X + offset.X, delta * smoothing);
            Camera.position.Y = Raymath.Lerp(Camera.position.Y, player.worldPosition.Y + offset.Y, delta * smoothing);
            //Enforce the camera bounds
            //Camera.position.X = Math.Clamp(Camera.position.X, minX, maxX);
            //Camera.position.Y = Math.Clamp(Camera.position.Y, minY, maxY);
        }
    }
}