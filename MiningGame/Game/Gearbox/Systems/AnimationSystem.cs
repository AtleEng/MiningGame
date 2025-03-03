using System.Numerics;
using CoreEngine;
using Engine;

namespace Animation
{
    //System that handels frame by frame animations
    public class AnimationSystem : GameSystem
    {
        public override void Update(float delta)
        {
            foreach (GameEntity gameEntity in Core.activeGameEntities) //loop thruogh all active entitys
            {
                Animator? animator = gameEntity.GetComponent<Animator>(); //Check if entity has animator
                if (animator != null)
                {
                    //Check if animation is valid and should play
                    if (animator.currentAnimation != null && animator.animations.ContainsKey(animator.currentAnimation) && animator.isPlaying)
                    {
                        //add to animations timer
                        animator.timer += delta;
                        //if animations frame is over one frame
                        if (animator.timer >= animator.animations[animator.currentAnimation].FrameDuration)
                        {
                            //calculate the new frame as int
                            animator.currentFrame = (animator.currentFrame + 1) % animator.animations[animator.currentAnimation].Frames.Length;
                            //Reset frame timer
                            animator.timer = 0;
                            //Set the correct spriteIndex
                            animator.sprite.FrameIndex = animator.animations[animator.currentAnimation].Frames[animator.currentFrame];

                            //Check if animation is done and shouldnt loop
                            if (animator.currentFrame == 0 && !animator.animations[animator.currentAnimation].loop)
                            {
                                animator.isPlaying = false;
                            }
                        }
                    }
                }
            }
        }
    }
}