#pragma once

#include "input.h"
#include "vaultEngine_lib.h"
#include "render_interface.h"

// ################################     Game Constants   ################################
constexpr int UPDATES_PER_SECOND = 60;
constexpr double UPDATE_DELAY = 1.0 / UPDATES_PER_SECOND;

constexpr int WORLD_WIDTH = 320;
constexpr int WORLD_HEIGHT = 180;

constexpr int TILESIZE = 8;
constexpr IVec2 WORLD_GRID = {WORLD_WIDTH/TILESIZE, WORLD_HEIGHT/TILESIZE};

// ################################     Game Structs   ################################

//input
enum GameInputType
{
    //Movement
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    JUMP,

    //Mouse
    PRIMARY,
    SECONDARY,
    MOUSE_MIDDLE,

    //UI
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

struct Player
{
    IVec2 pos, prevPos;
};


struct GameState
{
    float updateTimer;

    bool initialized = false;
    Player player;

    Array<IVec2, 21> tileCoords;
    Tile worldGrid[WORLD_GRID.x][WORLD_GRID.y];
    KeyMapping keyMappings[GAME_INPUT_COUNT];
};

// ################################     Game Globals   ################################
static GameState* gameState;

// ################################     Game Functions (Exposed)   ################################
void draw();
void fixed_update();

// ################################     Game Functions (Exposed)   ################################
extern "C"
{
    EXPORT_FN void update_game(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn, float dt);
}
