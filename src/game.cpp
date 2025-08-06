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


    draw_sprite(SPRITE_BLOCK, {0, 0});
}