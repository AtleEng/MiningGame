using System.Numerics;
using System.Collections.Generic;
using Raylib_cs;
using CoreEngine;
using Engine;

namespace UI
{
    //Component used to display Text on the UI
    public class UIText : Component, IRendable
    {
        //Controll which layer the text is being redered at (lower layer behind higher layer)
        public int Layer { get; set; } = 11;
        //the diplayed texts text
        public string text = "Enter text here:";
        //size of text
        public int fontSize = 50;
        //color of text
        public Color color = Color.White;
        //Posistion realative to entity
        public void Render()//What the sprite render call
        {
            // Get the screen position/size from the sprites world position/size
            Vector2 p = gameEntity.transform.worldPosition;
            Vector2 s = gameEntity.transform.worldSize;

            Raylib.DrawText(text, (int)p.X - (int)(s.X/2), (int)p.Y - (int)(s.Y/2), fontSize, color);
        }
        public override string PrintStats()//for debug parent tree (press F3)
        {
            return $"Pos: {gameEntity.transform.worldPosition}";
        }
    }
}