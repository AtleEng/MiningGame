#include "game.h"

#include "assets.h"

// ################################     Game Constants   ################################
constexpr int WORLD_WIDTH = 320;
constexpr int WORLD_HEIGHT = 180;

constexpr int TILESIZE = 8;

// ################################     Game Structures   ################################

// ################################     Game Functions   ################################

EXPORT_FN void update_game(GameState *gameStateIn, RenderData *renderDataIn, Input *inputIn)
{
    if (renderData != renderDataIn)
    {
        renderData = renderDataIn;
        input = inputIn;
        gameState = gameStateIn;
    }
    if (!gameState->initialized)
    {
        renderData->gameCamera.dimensions = {WORLD_WIDTH, WORLD_HEIGHT};
        gameState->initialized = true;
    }

    if(key_is_down(KEY_A))
    {
        gameState->playerPos.x -= 1;
    }
    if(key_is_down(KEY_D))
    {
        gameState->playerPos.x += 1;
    }
    if(key_is_down(KEY_W))
    {
        gameState->playerPos.y -= 1;
    }
    if(key_is_down(KEY_S))
    {
        gameState->playerPos.y += 1;
    }

    draw_sprite(SPRITE_BLOCK, gameState->playerPos);
}