namespace Engine
{
    public abstract class GameSystem
    {
        public List<Component> validComponents = new(); //Doesn't have a use right now
        public virtual void OnSystemStart() { } //Start of system
        public virtual void OnSystemUpdate(float delta) { } //Updates every frame
    }
    public interface IScript { } // Infterface for scriptes
}