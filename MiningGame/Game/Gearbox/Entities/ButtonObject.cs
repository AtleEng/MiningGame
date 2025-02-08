using System.Numerics;
using Raylib_cs;
using UI;

namespace Engine
{
    public class ButtonObject : GameEntity
    {
        public UIImage uIImage;
        public UIText uIText;
        public Button button;

        static Texture2D texture;

        //Add all diffrent components
        public ButtonObject(Action onKlick)
        {
            name = "Button";
            if (texture.Id == 0)
            {
                texture = Raylib.LoadTexture(@"Game\Project\Images\Button.png");
            }

            uIImage = new UIImage
            {
                spriteSheet = texture
            };
            AddComponent<UIImage>(uIImage);

            uIText = new UIText
            {
                Layer = 11,
                text = "Button",
                color = Color.Black
            };
            EntityManager.SpawnEntity(new TextBox(uIText), new Vector2(25, 25), Vector2.One, transform);

            button = new Button(onKlick, uIImage);
            AddComponent<Button>(button);
        }
    }
}