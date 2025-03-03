using System.Numerics;
using System.Collections.Generic;
using Raylib_cs;
using Animation;
using Physics;

namespace Engine
{
    public class Player : GameEntity
    {
        static Texture2D texture;
        //Add all diffrent components
        public Player()
        {
            name = "Player";
            if (texture.Id == 0)
            {
                texture = Raylib.LoadTexture(@"Game\Project\Images\PlayerSpriteSheet.png");
            }
            //sprite
            Sprite sprite = new Sprite
            {
                spriteSheet = texture,
                spriteGrid = new Vector2(4, 2),
                FrameIndex = 4
            };
            AddComponent<Sprite>(sprite);

            //animation
            Animator animator = new(sprite);

            animator.AddAnimation("Idle", new(new int[] { 4, 5 }, 0.5f, true));
            animator.AddAnimation("Run", new(new int[] { 0, 1, 2, 3 }, 0.1f, true));
            animator.AddAnimation("Fall", new(new int[] { 6, }, 0.1f, false));
            animator.AddAnimation("Jump", new(new int[] { 3 }, 0.1f, false));

            AddComponent<Animator>(animator);

            //physics
            PhysicsBody physicsBody = new PhysicsBody
            {
                dragX = 10f,
                dragY = 0,
                Gravity = new Vector2(0, 50),
                velocity = Vector2.Zero
            };
            AddComponent<PhysicsBody>(physicsBody);

            Collider collider = new Collider
            (
                false, 0
            )
            {
                scale = new Vector2(0.5f, 1)
            };
            AddComponent<Collider>(collider);

            //Other scripts
            PlayerMovement playerMovement = new();
            AddComponent<PlayerMovement>(playerMovement);

            AddComponent<CameraController>(new CameraController(transform));

            //ground check (child of player)
            Check check = new(2);
            EntityManager.SpawnEntity(check, new Vector2(0, 0.4f), new Vector2(0.5f, 0.5f), this.transform);
            playerMovement.groundCheck = check.GetComponent<Collider>();

            Check wallCheck = new(2);
            EntityManager.SpawnEntity(wallCheck, new Vector2(-0.3f, 0f), new Vector2(0.4f, 0.4f), this.transform);
            playerMovement.wallCheck = wallCheck.GetComponent<Collider>();
        }
    }
}