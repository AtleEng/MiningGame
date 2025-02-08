using UI;

namespace Engine
{
    public class TextBox : GameEntity
    {
        public UIText uIText;
        public TextBox(UIText text)
        {
            name = "TextBox";
            AddComponent<UIText>(text);
            uIText = text;
        }
    }
}