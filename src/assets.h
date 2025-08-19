#pragma once

#include "vaultEngine_lib.h"
// ################################     Assets Constants    ################################

// ################################     Assets Structs   ################################
enum SpriteID
{
    SPRITE_BLANK,
    SPRITE_BLOCK,

    SPRITE_COUNT
};

struct Sprite
{
    IVec2 atlasOffset;
    IVec2 spriteSize;
};

// ################################     Assets Functions   ################################
Sprite get_sprite(SpriteID spriteID)
{
    Sprite sprite = {};

    switch (spriteID)
    {
    case SPRITE_BLANK:
    {
        sprite.atlasOffset = {0, 0};
        sprite.spriteSize = {1, 1};
        break;
    }
    case SPRITE_BLOCK:
    {
        sprite.atlasOffset = {8, 0};
        sprite.spriteSize = {8, 8};
        break;
    }
    }
    return sprite;
}
