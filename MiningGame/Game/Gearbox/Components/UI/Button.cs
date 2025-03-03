using System.Numerics;
using System.Collections.Generic;
using Raylib_cs;
using CoreEngine;
using Engine;

namespace UI
{
    //this is not used so I wont explain it
    public class Button : Component, IScript
    {
        Action OnKilcked;
        public bool isHovering;

        UIImage image;
        public Color normalColor = Color.White;
        public Color hoverColor = Color.Red;

        Sound sound = Raylib.LoadSound(@"Game\Project\Audio\metronome-85688.mp3");
        public Button(Action KlickAction, UIImage image)
        {
            OnKilcked = KlickAction;
            this.image = image;
        }
        public override void Update(float delta)
        {
             // Get the screen position/size from the sprites world position/size
            Vector2 p = gameEntity.transform.worldPosition;
            Vector2 s = gameEntity.transform.worldSize;

            //Calculate rectangle the sprite is rendered in from p and s
            Rectangle destRec = new Rectangle(
            (int)p.X - (int)(s.X / 2), (int)p.Y - (int)(s.Y / 2), //pos
            (int)s.X, (int)s.Y //size
            );

            Vector2 mPos = WorldSpace.GetUIMousePos();
            if (Raylib.CheckCollisionPointRec
                (mPos, destRec))
            {
                isHovering = true;
                image.colorTint = hoverColor;
                if (Raylib.IsMouseButtonPressed(0))
                {
                    Raylib.PlaySound(sound);
                    OnKilcked();
                }
            }
            else
            {
                isHovering = false;
                image.colorTint = normalColor;
            }
        }
    }
}