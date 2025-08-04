#include "game.h"

#include "assets.h"

// ################################     Game Constants   ################################

// ################################     Game Structures   ################################

// ################################     Game Functions   ################################

EXPORT_FN void update_game(RenderData* renderDataIn, Input* inputIn)
{
    if(renderData != renderDataIn)
    {
        renderData = renderDataIn;
        input = inputIn;
    }

    for (int x = 0; x < 10; x++)
    {
        for (int y = 0; y < 10; y++)
        {
            draw_sprite(SPRITE_BLOCK, {100.0f * x, 100.0f * y}, {100.0f, 100.0f});
        }
    }

    
}