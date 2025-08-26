#pragma once

#include "../engine_utils/assets.h"
#include "../vaultEngine_lib.h"

// ################################     Render Constants    ################################
int RENDER_OPTION_FLIP_X = BIT(0);
int RENDER_OPTION_FLIP_Y = BIT(1);

// ################################     Render Structs   ################################
struct OrthographicCamera2D
{
    float zoom = 1.0f;
    Vec2 dimensions;
    Vec2 position;
};
struct DrawData
{
    int animationIdx;
    int renderOptions;
};
struct RenderTransform
{
    Vec2 pos;
    Vec2 size;
    IVec2 atlasOffset;
    IVec2 spriteSize;
    int animationIdx;
    // TODO this fucks up tile render for some reson?
    int renderOptions;
};

struct RenderData
{
    OrthographicCamera2D gameCamera;
    OrthographicCamera2D uiCamera;

    Array<RenderTransform, 1000> transforms;
};

// ################################     Render Globals   ################################
static RenderData *renderData;

// ################################     Render Utils   ################################
IVec2 screen_to_world(IVec2 screenPos)
{
    OrthographicCamera2D camera = renderData->gameCamera;

    int xPos = (float)screenPos.x /
               (float)input->screenSize.x *
               camera.dimensions.x; // [0; dimensions.x]

    // Offset using dimensions and position
    xPos += -camera.dimensions.x / 2.0f + camera.position.x;

    int yPos = (float)screenPos.y /
               (float)input->screenSize.y *
               camera.dimensions.y; // [0; dimensions.y]

    // Offset using dimensions and position
    yPos += camera.dimensions.y / 2.0f + camera.position.y;

    return {xPos, yPos};
}

//? return a index for which frame a animation should have
int animate_spritesheet(float *time, int frameCount, float duration = 1.0f)
{
    while (*time > duration)
    {
        *time -= duration;
    }
    int animIdx = (int)((*time / duration) * frameCount);
    // clamp to existing frames
    if (animIdx >= frameCount)
    {
        animIdx = frameCount - 1;
    }
    return animIdx;
}

// ################################     Render Functions   ################################
void draw_quad(RenderTransform transform)
{
    renderData->transforms.add(transform);
}

void draw_quad(Vec2 pos, Vec2 size) // This draws the first pixel in the texture atlas (top-left)
{
    RenderTransform transform = {};
    transform.pos = pos - size / 2.0f;
    transform.size = size;
    transform.atlasOffset = {0, 0};
    transform.spriteSize = {1, 1};

    renderData->transforms.add(transform);
}

void draw_sprite(SpriteID spriteID, Vec2 pos, DrawData data = {})
{
    Sprite sprite = get_sprite(spriteID);

    sprite.atlasOffset.x += data.animationIdx * sprite.spriteSize.x;

    RenderTransform transform = {};
    transform.pos = pos - vec_2(sprite.spriteSize) / 2.0f;
    transform.size = vec_2(sprite.spriteSize);
    transform.atlasOffset = sprite.atlasOffset;
    transform.spriteSize = sprite.spriteSize;
    transform.renderOptions = data.renderOptions;

    renderData->transforms.add(transform);
}
void draw_sprite(SpriteID spriteID, IVec2 pos, DrawData data = {})
{
    draw_sprite(spriteID, vec_2(pos), data);
}