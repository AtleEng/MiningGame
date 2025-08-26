#pragma once

#include "../engine_utils/input.h"
#include "../vaultEngine_lib.h"
#include "../render/render_interface.h"
#include "../engine_utils/assets.h"
#include "../engine_utils/sound.h"

// ################################     Game Constants   ################################
constexpr int UPDATES_PER_SECOND = 60;
constexpr double UPDATE_DELAY = 1.0 / UPDATES_PER_SECOND;

constexpr int WORLD_WIDTH = 320;
constexpr int WORLD_HEIGHT = 180;

constexpr int TILESIZE = 8;
constexpr IVec2 WORLD_GRID = {WORLD_WIDTH / TILESIZE, (WORLD_HEIGHT / TILESIZE) + 1};

// ################################     Game Structs   ################################

// input
enum GameInputType
{
    // Movement
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    JUMP,

    // Mouse
    PRIMARY,
    SECONDARY,
    MOUSE_MIDDLE,

    // UI
    MENU,

    GAME_INPUT_COUNT
};

struct KeyMapping
{
    Array<KeyCodeID, 3> keys;
};

struct Tile
{
    int neighbourMask;
    bool isVisible;
};

enum PlayerAnimState
{
    PLAYER_ANIM_IDLE,
    PLAYER_ANIM_RUN,
    PLAYER_ANIM_JUMP,
    PLAYER_ANIM_COUNT
};

struct Transform
{
    IVec2 pos, prevPos;

    IVec2 aabb;

    Vec2 speed;
    Vec2 remainder;

    bool hasCollided;
    bool isGrounded;

    int renderOptions;
    float frameTime;
    PlayerAnimState animState;
    SpriteID animationSprites[PLAYER_ANIM_COUNT];
};

struct GameState
{
    float updateTimer;

    bool initialized = false;
    Transform player;

    Array<IVec2, 21> tileCoords;
    Tile worldGrid[WORLD_GRID.x][WORLD_GRID.y];
    KeyMapping keyMappings[GAME_INPUT_COUNT];
};

// ################################     Game Globals   ################################
static GameState *gameState;

// ################################     Game Functions (Exposed)   ################################
void init();
void fixed_update();
void draw();

// ################################     Game Functions (Exposed)   ################################
extern "C"
{
    EXPORT_FN void update_game(GameState *gameStateIn, RenderData *renderDataIn, Input *inputIn, SoundState *soundStateIn, float dt);
}
