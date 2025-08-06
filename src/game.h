#pragma once

#include "input.h"
#include "vaultEngine_lib.h"
#include "render_interface.h"

// ################################     Game Constants   ################################
constexpr int tset = 5;

// ################################     Game Structures   ################################
struct GameState
{
    bool initialized = false;
    IVec2 playerPos;
};

// ################################     Game Globals   ################################
static GameState* gameState;

// ################################     Game Functions (Exposed)   ################################
extern "C"
{
    EXPORT_FN void update_game(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn);
}
