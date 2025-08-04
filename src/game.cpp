#include "assets.h"
#include "render_interface.h"
#include "vaultEngine_lib.h"

// ################################     Game Constants   ################################

// ################################     Game Structures   ################################

// ################################     Game Functions   ################################

void updateGame()
{
    for (int x = 0; x < 10; x++)
    {
        for (int y = 0; y < 10; y++)
        {
            draw_sprite(SPRITE_BLOCK, {100.0f * x, 100.0f * y}, {100.0f, 100.0f});
        }
    }

    
}