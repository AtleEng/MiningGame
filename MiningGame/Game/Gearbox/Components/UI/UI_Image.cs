using System.Numerics;
using Raylib_cs;

namespace Engine
{
    //Component used to display sprites on the UI
    public class UIImage : Component, IRendable
    {
        //Controll which layer sprite is being redered at (lower layer behind higher layer)
        public int Layer { get; set; } = 10;

        //The spriteSheet, load from PNG
        public Texture2D spriteSheet;
        //The method to slice spriteSheet
        public Vector2 spriteGrid = Vector2.One;
        //which grid in spriteGrid is currently active
        int frameIndex;
        public int FrameIndex
        {
            get
            {
                return frameIndex;
            }
            set
            {
                if (value > spriteGrid.X * spriteGrid.Y - 1)
                {
                    frameIndex = 0;
                }
                else if (value < 0)
                {
                    frameIndex = (int)(spriteGrid.X * spriteGrid.Y - 1);
                }
                else
                {
                    frameIndex = value;
                }
            }
        }
        //Color tint of rendered sprite
        public Color colorTint = Color.White;
        //Bools to flip sprite
        public bool isFlipedY;
        public bool isFlipedX;

        public override string PrintStats()//for debug parent tree (press F3)
        {
            return $"SpriteGrid: {spriteGrid} FrameIndex: {frameIndex} Layer: {Layer}";
        }

        public void Render()//What the sprite render call
        {
            // Get the screen position/size from the sprites world position/size
            Vector2 p = gameEntity.transform.worldPosition;
            Vector2 s = gameEntity.transform.worldSize;

            //Calculate rectangle the sprite is rendered in from p and s
            Rectangle destRec = new Rectangle(
            (int)p.X - (int)(s.X / 2), (int)p.Y - (int)(s.Y / 2), //pos
            (int)s.X, (int)s.Y //size
            );
            //check so the sprite has a spriteSheet
            if (spriteSheet.Id != 0)
            {
                //Get all variables from sprite and use them "cut out" sprite
                int flipX = isFlipedX ? -1 : 1;
                int flipY = isFlipedY ? -1 : 1;

                int i = FrameIndex;

                int x = (int)spriteGrid.X;
                int y = (int)spriteGrid.Y;

                float gridSizeX = spriteSheet.Width / x;
                float gridSizeY = spriteSheet.Height / y;

                int posX = i % x;
                int posY = i / x;
                //create the cutter rectangle
                Rectangle source = new Rectangle(
                    (int)(posX * gridSizeX),
                    (int)(posY * gridSizeY),
                    spriteSheet.Width * flipX / spriteGrid.X,
                spriteSheet.Height * flipY / spriteGrid.Y
                );
                //Draw the sprite with both rects
                Raylib.DrawTexturePro(spriteSheet, source, destRec, Vector2.Zero, 0, colorTint);
            }
            else
            {
                Raylib.DrawRectangleRec(destRec, colorTint);
            }
        }
    }
}