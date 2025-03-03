using System.Numerics;
using System.Collections.Generic;
using Raylib_cs;
using Engine;
using Physics;
using Animation;

namespace Engine
{
    public class PlayerMovement : Component, IScript
    {
        #region Variables
        #region XMovement
        float moveSpeed = 100;
        float airSpeed = 15;

        float groundDrag = 10;
        float airDrag = 1;
        private float moveInput;
        #endregion

        #region NormalJump
        private float jumpForce = 10;

        private float jumpTime = 0.3f;
        private float jumpTimeCounter;

        private bool isJumping;

        private float hangTime = 0.1f;
        private float hangTimeCounter;

        private float jumpBufferLength = 0.1f;
        private float jumpBufferLengthCounter;

        public Transform feetPos;

        public Collider groundCheck;
        #endregion

        #region WallJump
        bool isWallJumping = true;
        float wallJumpTime = 0.15f;
        float wallJumpTimer;

        public Collider wallCheck;
        #endregion

        //Physics
        float maxVelocityX = 10;
        float maxVelocityY = 30;
        public PhysicsBody pB;
        public Collider collider;

        //Sprite & Animation
        Sprite sprite;
        Animator anim;
        PlayerStates playerState = PlayerStates.idle;

        //Audio
        Sound jumpSound = Raylib.LoadSound(@"Game\Project\Audio\cartoon-jump-6462.mp3");
        Sound deathSound = Raylib.LoadSound(@"Game\Project\Audio\jump-climb-or-damage-sound-f-95942.mp3");

        public event EventHandler PlayerDie;
        #endregion
        public override void Start()
        {
            //Get all components script needs
            pB = gameEntity.GetComponent<PhysicsBody>();
            anim = gameEntity.GetComponent<Animator>();
            sprite = gameEntity.GetComponent<Sprite>();
            collider = gameEntity.GetComponent<Collider>();
        }

        void HandleAnimation() //This method handles animation states
        {
            if (groundCheck.isColliding)
            {
                if (pB.velocity.X != 0)
                {
                    //if player is on the ground and moving
                    if (playerState != PlayerStates.running)
                    {
                        playerState = PlayerStates.running;
                        anim.PlayAnimation("Run");
                    }
                }
                else
                {
                    //if player is on the ground and standing still
                    if (playerState != PlayerStates.idle)
                    {
                        playerState = PlayerStates.idle;
                        anim.PlayAnimation("Idle");
                    }
                }
            }
            else
            {
                if (pB.velocity.Y < 0)
                {
                    //if player is on the ground and pressing space
                    if (playerState != PlayerStates.jump)
                    {
                        playerState = PlayerStates.jump;
                        anim.PlayAnimation("Jump");
                    }
                }
                else
                {
                    //if player is on the wall and moving
                    if (wallCheck.isColliding && moveInput != 0)
                    {
                        if (playerState != PlayerStates.wallGliding)
                        {
                            playerState = PlayerStates.wallGliding;
                        }
                    }
                    else
                    {
                        //Else the player falls
                        if (playerState != PlayerStates.fall)
                        {
                            playerState = PlayerStates.fall;
                            anim.PlayAnimation("Fall");
                        }
                    }
                }
            }
        }
        public override void Update(float delta)
        {
            HandleAnimation(); //set the correct animation

            JumpInputs(delta); //Get input for jumping

            if (isWallJumping) //If walljumping is true then walljump
            {
                WallJump(delta); // Controll walljump
            }
            else
            {
                XInput(); //If not walljumping the move correctly
            }
            xMovement(delta); //Controll x Movement
            Jump(delta); // Controll jump

            //clamp velocity of player
            pB.velocity.X = Math.Clamp(pB.velocity.X, -maxVelocityX, maxVelocityX);
            pB.velocity.Y = Math.Clamp(pB.velocity.Y, -maxVelocityY, maxVelocityY);

            if (groundCheck.isColliding)
            {
                //Hanterar spelarens hastighet
                if (Math.Abs(pB.velocity.X) < 1) { pB.velocity.X = 0; }
            }

            if (gameEntity.transform.position.Y > 1000)
            {
                Kill();
            }
        }
        void XInput() //get the x movement input
        {
            moveInput = 0;
            if (Raylib.IsKeyDown(KeyboardKey.D))
            {
                moveInput++;
            }
            if (Raylib.IsKeyDown(KeyboardKey.A))
            {
                moveInput--;
            }
        }
        void JumpInputs(float delta)
        {
            //manage jump buffer
            if (Raylib.IsKeyPressed(KeyboardKey.Space))// set the input buffer timer
            {
                jumpBufferLengthCounter = jumpBufferLength;
            }
            else //Count down the timer
            {
                jumpBufferLengthCounter -= delta;
            }
            //Slow down player if wallgliding
            if (playerState == PlayerStates.wallGliding)
            {
                pB.dragY = 20;
                if (jumpBufferLengthCounter >= 0)
                {
                    moveInput *= -1;
                    pB.dragY = 0;
                    isWallJumping = true;
                }
            }
            else
            {
                //set correct drag
                pB.dragY = 0;

                if (groundCheck.isColliding) //if player is on the ground set hangtime (Makes you able to jump a short while after leaving the ground)
                {
                    hangTimeCounter = hangTime;
                }
                else //count down timer
                {
                    hangTimeCounter -= delta;
                }
            }
            if (jumpBufferLengthCounter >= 0 && (hangTimeCounter > 0 || playerState == PlayerStates.wallGliding)) //Check so player should be able to jump
            {
                isJumping = true;
                jumpTimeCounter = jumpTime;
                jumpBufferLengthCounter = 0;
                hangTimeCounter = 0;
                Raylib.PlaySound(jumpSound);
            }
            //Check if holding down jumpbutton => contine adding velocity
            if (Raylib.IsKeyDown(KeyboardKey.Space))
            {
                if (jumpTimeCounter > 0) //JumpTiem timer
                {
                    jumpTimeCounter -= delta;
                }
                else //When jumpTime is done
                {
                    isJumping = false; //Stop jumping
                }
            }
            else //If jumpButton is up
            {
                isJumping = false; //Stop Jumping
            }
        }
        void xMovement(float delta)
        {
            if (groundCheck.isColliding || isWallJumping) //If walking on the ground
            {
                pB.dragX = groundDrag;
                pB.velocity.X += moveInput * moveSpeed * delta;
            }
            else //If moving in the air
            {
                pB.dragX = airDrag;
                pB.velocity.X += moveInput * airSpeed * delta;
            }
            //Flip the sprite and collider
            if (moveInput > 0) //If moving right
            {
                wallCheck.gameEntity.transform.position.X = 0.3f;

                sprite.isFlipedX = false;
            }
            else if (moveInput < 0) //if moving left
            {
                wallCheck.gameEntity.transform.position.X = -0.3f;

                sprite.isFlipedX = true;
            }
        }
        void Jump(float delta)
        {
            if (isJumping) //Add force if jumping
            {
                pB.velocity.Y = -jumpForce * delta * 60;
            }
        }
        void WallJump(float delta) //Walljumping timer
        {
            wallJumpTimer -= delta;
            if (wallJumpTimer <= 0)
            {
                wallJumpTimer = wallJumpTime;
                isWallJumping = false;
            }
        }

        public override void OnCollision(Collider other) //Look if entity collide with a entity with IKill and if => restart level
        {
            /*
            if (other.gameEntity is IKill)
            {
                Kill();
            }
            */
        }
        void Kill()
        {
            Raylib.PlaySound(deathSound);
            PlayerDie?.Invoke(this, EventArgs.Empty);
            //gM.ChangeLevel(LoadingManager.CurrentLevel);
        }
        public enum PlayerStates //The diffrent states of the player 
        {
            idle, running, jump, fall, wallGliding
        }
    }
}
