#pragma once

#include "input.h"
#include "vaultEngine_lib.h"
#include "render_interface.h"

// ################################     Game Constants   ################################

// ################################     Game Structures   ################################

// ################################     Game Functions (Exposed)   ################################
extern "C"
{
    EXPORT_FN void update_game(RenderData* renderDataIn, Input* inputIn);
}
