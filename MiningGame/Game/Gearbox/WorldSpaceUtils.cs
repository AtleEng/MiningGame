using System.Numerics;
using Raylib_cs;
using CoreEngine;
using Graphics;

namespace Engine
{
    //Utils class for world and screen space
    public static class WorldSpace
    {
        public static int pixelsPerUnit = 40;
        static RenderSystem RenderSystem => (RenderSystem)Core.systems[Core.systems.Count - 1]; //cache the renderSystem
        public static Vector2 GetGameMousePos() // Update the game mouse (It is bound to the game and the center of screen is 0,0)
        {
            // Game mouse position
            Vector2 gameMousePosition;

            // Get the mouse position
            Vector2 mousePosition = Raylib.GetMousePosition();

            // Calculate the game mouse position adjusted to the game window
            gameMousePosition.X = (mousePosition.X - (Raylib.GetScreenWidth() - (WindowSettings.gameScreenWidth * RenderSystem.scale)) * 0.5f) / RenderSystem.scale;
            gameMousePosition.Y = (mousePosition.Y - (Raylib.GetScreenHeight() - (WindowSettings.gameScreenHeight * RenderSystem.scale)) * 0.5f) / RenderSystem.scale;

            // Clamp the game mouse position to the game window boundaries
            gameMousePosition.X = Math.Clamp(gameMousePosition.X, 0f, WindowSettings.gameScreenWidth);
            gameMousePosition.Y = Math.Clamp(gameMousePosition.Y, 0f, WindowSettings.gameScreenHeight);

            gameMousePosition.X = (gameMousePosition.X - WindowSettings.gameScreenWidth / 2) / Camera.zoom / pixelsPerUnit;
            gameMousePosition.Y = (gameMousePosition.Y - WindowSettings.gameScreenHeight / 2) / Camera.zoom / pixelsPerUnit;

            gameMousePosition += Camera.position;

            return gameMousePosition;
        }

        public static Vector2 GetUIMousePos() // Update the game mouse (It is bound to the game and the center of screen is 0,0)
        {
            // Game mouse position
            Vector2 uiMousePosition;

            // Get the mouse position
            Vector2 mousePosition = Raylib.GetMousePosition();

            // Calculate the game mouse position adjusted to the game window
            uiMousePosition.X = (mousePosition.X - (Raylib.GetScreenWidth() - (WindowSettings.gameScreenWidth * RenderSystem.scale)) * 0.5f) / RenderSystem.scale;
            uiMousePosition.Y = (mousePosition.Y - (Raylib.GetScreenHeight() - (WindowSettings.gameScreenHeight * RenderSystem.scale)) * 0.5f) / RenderSystem.scale;

            // Clamp the game mouse position to the game window boundaries
            uiMousePosition.X = Math.Clamp(uiMousePosition.X, 0f, WindowSettings.gameScreenWidth);
            uiMousePosition.Y = Math.Clamp(uiMousePosition.Y, 0f, WindowSettings.gameScreenHeight);

            return uiMousePosition;
        }
        public static int BaseUnitsToPixels(float units)
        {
            int pixels = (int)(units * pixelsPerUnit);
            return pixels;
        }
        public static float PixelsToBaseUnits(int pixels)
        {
            float units = (float)pixels / pixelsPerUnit;
            return units;
        }
        public static Vector2 ConvertToCameraPosition(Vector2 v) //Gets the screenPosition of a vector in worldPosition
        {
            v.X = (v.X - Camera.position.X) * pixelsPerUnit * Camera.zoom + WindowSettings.gameScreenWidth / 2;
            v.Y = (v.Y - Camera.position.Y) * pixelsPerUnit * Camera.zoom + WindowSettings.gameScreenHeight / 2;

            return v;
        }
        public static Vector2 ConvertToCameraSize(Vector2 v) //Gets the screenSize of a vector in worldSize
        {
            return v * Camera.zoom * pixelsPerUnit;
        }
    }

    public static class WindowSettings
    {
        readonly public static int startWindowWidth = 800;
        readonly public static int startWindowHeight = 450;

        public static int gameScreenWidth = 1120;
        public static int gameScreenHeight = 800;
    }
    public static class Camera
    {
        public static Vector2 position = new();
        public static float zoom = 1;
    }
}