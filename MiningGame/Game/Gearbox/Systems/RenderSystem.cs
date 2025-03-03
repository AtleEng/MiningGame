using System.Collections.Generic;
using System.Numerics;
using Raylib_cs;

using CoreEngine;
using Engine;
using Physics;

namespace Graphics
{
    //System that handle all diffrent render (sprite, text....)
    public class RenderSystem : GameSystem
    {
        public float scale; //gameScreen scale
        //offest
        int offsetX;
        int offsetY;

        RenderTexture2D target = new();

        float gameScreenWidth;
        float gameScreenHeight;

        List<IRendable> allRenderObjects = new();

        public override void Start()
        {
            //set all window things
            System.Console.WriteLine("Innit window");
            Raylib.SetConfigFlags(ConfigFlags.ResizableWindow);
            Raylib.SetConfigFlags(ConfigFlags.VSyncHint);

            Raylib.InitWindow(WindowSettings.startWindowWidth, WindowSettings.startWindowHeight, "Game Window");
            Raylib.SetWindowMinSize(400, 300);

            Raylib.SetTargetFPS(60);

            Raylib.SetExitKey(KeyboardKey.Null);

            target = Raylib.LoadRenderTexture(WindowSettings.gameScreenWidth, WindowSettings.gameScreenHeight);
            SetValuesOfWindow();
        }
        public override void Update(float delta)
        {
            //If window is change recalc all window values
            if (Raylib.IsWindowResized())
            {
                SetValuesOfWindow();
            }

            Raylib.BeginTextureMode(target);
            //Background color
            Raylib.ClearBackground(new Color(29, 43, 83, 255));
            //Render all sprites
            RenderAll();

            Raylib.EndTextureMode();

            Raylib.BeginDrawing();

            //Bar color
            Raylib.ClearBackground(new Color(0, 0, 0, 255));
            //draw the texture with all things on
            Raylib.DrawTexturePro(target.Texture,
    new Rectangle(0.0f, 0.0f, (float)target.Texture.Width, (float)-target.Texture.Height),
    new Rectangle(offsetX, offsetY, gameScreenWidth * scale, gameScreenHeight * scale),
    new Vector2(0, 0), 0.0f, Color.White);

            Raylib.EndDrawing();

            if (Core.shouldClose) // close logic
            {
                Raylib.UnloadRenderTexture(target);
                Raylib.CloseWindow();
            }

            if (Raylib.WindowShouldClose())
            {
                Core.shouldClose = true;
            }
        }
        void RenderAll() //Used to render all IRender
        {
            foreach (GameEntity gameEntity in Core.activeGameEntities)
            {
                IRendable? r = gameEntity.GetComponentInterface<IRendable>(); // get all active IRendables
                if (r != null)
                {
                    allRenderObjects.Add(r);// add to list
                }
            }
            allRenderObjects.Sort((a, b) => a.Layer.CompareTo(b.Layer)); // sort by layer
            foreach (IRendable rB in allRenderObjects) //loop all IRendables
            {
                rB.Render(); //call its Render method
            }
            allRenderObjects.Clear(); //clear list for next update
        }
        void SetValuesOfWindow() //set all settings for the window
        {
            gameScreenWidth = WindowSettings.gameScreenWidth;
            gameScreenHeight = WindowSettings.gameScreenHeight;

            float screenAspectRatio = (float)Raylib.GetScreenWidth() / Raylib.GetScreenHeight(); //Get screen ratio
            float gameAspectRatio = gameScreenWidth / gameScreenHeight; // calc aspectRatio

            if (screenAspectRatio > gameAspectRatio)
            {
                // Window is wider than the game screen => fix so gameWindow match aspektRatio
                scale = Raylib.GetScreenHeight() / gameScreenHeight;
                offsetX = (int)((Raylib.GetScreenWidth() - (gameScreenWidth * scale)) * 0.5f);
                offsetY = 0;
            }
            else
            {
                // Window is taller than the game screen => fix so gameWindow match aspektRatio
                scale = (float)Raylib.GetScreenWidth() / gameScreenWidth;
                offsetX = 0;
                offsetY = (int)((Raylib.GetScreenHeight() - (gameScreenHeight * scale)) * 0.5f);
            }
        }

        void DisplayGrid() //Creates a grid (for debugging)
        {
            int gridSize = 100;
            Vector2 spX = WorldSpace.ConvertToCameraPosition(new Vector2(gridSize, 0));

            Vector2 epX = WorldSpace.ConvertToCameraPosition(new Vector2(-gridSize, 0));
            Raylib.DrawLine((int)spX.X, (int)spX.Y, (int)epX.X, (int)epX.Y, Color.Red);

            Vector2 spY = WorldSpace.ConvertToCameraPosition(new Vector2(0, gridSize));

            Vector2 epY = WorldSpace.ConvertToCameraPosition(new Vector2(0, -gridSize));
            Raylib.DrawLine((int)spY.X, (int)spY.Y, (int)epY.X, (int)epY.Y, Color.Blue);

            for (int x = -gridSize; x < gridSize; x++)
            {
                Vector2 sp = WorldSpace.ConvertToCameraPosition(new Vector2(x + 0.5f, gridSize + 0.5f));

                Vector2 ep = WorldSpace.ConvertToCameraPosition(new Vector2(x + 0.5f, -gridSize - 0.5f));
                Raylib.DrawLine((int)sp.X, (int)sp.Y, (int)ep.X, (int)ep.Y, Color.RayWhite);
            }
            for (int y = -gridSize; y < gridSize; y++)
            {
                Vector2 sp = WorldSpace.ConvertToCameraPosition(new Vector2(gridSize + 0.5f, y + 0.5f));

                Vector2 ep = WorldSpace.ConvertToCameraPosition(new Vector2(-gridSize - 0.5f, y + 0.5f));
                Raylib.DrawLine((int)sp.X, (int)sp.Y, (int)ep.X, (int)ep.Y, Color.RayWhite);
            }
        }
    }
}

