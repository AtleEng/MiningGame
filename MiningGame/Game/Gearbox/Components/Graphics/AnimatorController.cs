using Engine;

namespace Animation
{
    //Component used to play frame by frame animations
    public class Animator : Component
    {
        //Sprite reference
        public Sprite sprite;
        //all animations in animator
        public Dictionary<string, Animation> animations;
        //String to access animation
        public string currentAnimation = "";
        //current frame of animation
        public int currentFrame;
        //animation timer (change per animation)
        public float timer;
        //Check to see if animation is playing (otherwise still image)
        public bool isPlaying;

        //Constructor
        public Animator(Sprite sprite)
        {
            this.sprite = sprite;
            animations = new Dictionary<string, Animation>();
        }
        //this function is used to add new animations
        public void AddAnimation(string name, Animation animation)
        {
            animations[name] = animation;
        }
        //this function is used to play animations
        public void PlayAnimation(string name)
        {
            //check if animation exist
            if (animations.ContainsKey(name))
            {
                //tell system an animation is playing, and set which animation to play
                isPlaying = true;
                sprite.FrameIndex = animations[name].Frames[0];
                currentAnimation = name;
                currentFrame = 0;
                timer = 0;
            }//else debug message
            else { System.Console.WriteLine($"Animation {name} for {gameEntity.name} doesn't exist!!!"); }
        }
        public override string PrintStats() //for debug parent tree (press F3)
        {
            string animationsText = "";
            foreach (string t in animations.Keys)
            {
                animationsText += $"{t} ";
            }
            return $"Animations < {animationsText}> CurrentAnimation: {currentAnimation} IsPlaying{isPlaying}";
        }
    }
    public class Animation //class that stores a animation (a collection of diffrent images and setting for playing it)
    {
        public int[] Frames { get; set; }
        public float FrameDuration { get; set; }
        public bool loop;

        public Animation(int[] frames, float frameDuration, bool loop)
        {
            Frames = frames;
            FrameDuration = frameDuration;
            this.loop = loop;
        }
    }
}