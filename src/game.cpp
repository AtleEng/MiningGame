#include "game.h"

#include "assets.h"

// ################################     Game Constants   ################################

// ################################     Game Structs   ################################

// ################################     Game Functions   ################################
bool just_pressed(GameInputType type)
{
  KeyMapping mapping = gameState->keyMappings[type];
  for (int idx = 0; idx < mapping.keys.count; idx++)
  {
    if (input->keys[mapping.keys[idx]].justPressed)
    {
      return true;
    }
  }

  return false;
}

bool is_down(GameInputType type)
{
  KeyMapping mapping = gameState->keyMappings[type];
  for (int idx = 0; idx < mapping.keys.count; idx++)
  {
    if (input->keys[mapping.keys[idx]].isDown)
    {
      return true;
    }
  }

  return false;
}

IVec2 get_grid_pos(IVec2 worldPos)
{
  return {worldPos.x / TILESIZE, worldPos.y / TILESIZE};
}

Tile *get_tile(int x, int y)
{
  Tile *tile = nullptr;

  if (x >= 0 && x < WORLD_GRID.x && y >= 0 && y < WORLD_GRID.y)
  {
    tile = &gameState->worldGrid[x][y];
  }

  return tile;
}

Tile *get_tile(IVec2 worldPos)
{
  IVec2 gridPos = get_grid_pos(worldPos);
  return get_tile(gridPos.x, gridPos.y);
}

// ################################     Game Functions (exposed)   ################################

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
    renderData->gameCamera.position.x = 160;
    renderData->gameCamera.position.y = -90;

    gameState->initialized = true;

    // Key Mappings
    {
      gameState->keyMappings[MOVE_UP].keys.add(KEY_W);
      gameState->keyMappings[MOVE_UP].keys.add(KEY_UP);
      gameState->keyMappings[MOVE_LEFT].keys.add(KEY_A);
      gameState->keyMappings[MOVE_LEFT].keys.add(KEY_LEFT);
      gameState->keyMappings[MOVE_DOWN].keys.add(KEY_S);
      gameState->keyMappings[MOVE_DOWN].keys.add(KEY_DOWN);
      gameState->keyMappings[MOVE_RIGHT].keys.add(KEY_D);
      gameState->keyMappings[MOVE_RIGHT].keys.add(KEY_RIGHT);

      gameState->keyMappings[MOUSE_LEFT].keys.add(KEY_MOUSE_LEFT);
      gameState->keyMappings[MOUSE_RIGHT].keys.add(KEY_MOUSE_RIGHT);

      gameState->keyMappings[JUMP].keys.add(KEY_SPACE);
      gameState->keyMappings[MENU].keys.add(KEY_ESCAPE);
    }
  }

  if (is_down(MOUSE_LEFT))
  {
    IVec2 mousePosWorld = input->mousePosWorld;
    Tile *tile = get_tile(mousePosWorld);
    if (tile)
    {
      tile->isVisible = true;
    }
  }
  if (is_down(MOUSE_RIGHT))
  {
    IVec2 mousePosWorld = input->mousePosWorld;
    Tile *tile = get_tile(mousePosWorld);
    if (tile)
    {
      tile->isVisible = false;
    }
  }

  // Draw tileset
  {
    for (int y = 0; y < WORLD_GRID.y; y++)
    {
      for (int x = 0; x < WORLD_GRID.x; x++)
      {
        Tile *tile = get_tile(x, y);

        if (!tile->isVisible)
        {
          continue;
        }

        Vec2 tilePos = {
            x * (float)TILESIZE + (float)TILESIZE / 2.0f,
            y * (float)TILESIZE + (float)TILESIZE / 2.0f,
        };
        draw_quad(tilePos, {8,8});
      }
    }
  }

  if (is_down(MOVE_LEFT))
  {
    gameState->playerPos.x -= 1;
  }
  if (is_down(MOVE_RIGHT))
  {
    gameState->playerPos.x += 1;
  }
  if (is_down(MOVE_UP))
  {
    gameState->playerPos.y -= 1;
  }
  if (is_down(MOVE_DOWN))
  {
    gameState->playerPos.y += 1;
  }

  draw_sprite(SPRITE_BLOCK, gameState->playerPos);
}