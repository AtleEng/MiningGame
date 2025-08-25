#include "vaultEngine_lib.h"
#include "engine_utils/input.h"

#include "game/game.h"
#include "engine_utils/sound.h"

#include "platform/platform.h"

#define APIENTRY
#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

#ifdef _WIN32
#include "platform/win32_platform.cpp"
#endif

#include "render/gl_renderer.cpp"

// ################################     Game DLL    ################################
// this is the pointer to update_game in game.cpp
typedef decltype(update_game) update_game_type;
static update_game_type *update_game_ptr;

// ################################     Cross Plaform functions    ################################
// Used to get Delta Time
#include <chrono>
double get_delta_time();

void reload_game_dll(BumpAllocator *transientStorage);

int main()
{
    float dt = get_delta_time();

    BumpAllocator transientStorage = make_bump_allocator(MB(50));
    BumpAllocator persistentStorage = make_bump_allocator(MB(256));

    input = (Input *)bump_alloc(&persistentStorage, sizeof(Input));
    if (!input)
    {
        LOG_ERROR("Failed to allocate Input");
    }

    renderData = (RenderData *)bump_alloc(&persistentStorage, sizeof(RenderData));
    if (!renderData)
    {
        LOG_ERROR("Failed to allocate Renderdata");
    }

    gameState = (GameState *)bump_alloc(&persistentStorage, sizeof(GameState));
    if (!gameState)
    {
        LOG_ERROR("Failed to allocate GameState");
        return -1;
    }
    soundState = (SoundState *)bump_alloc(&persistentStorage, sizeof(GameState));
    if (!soundState)
    {
        LOG_ERROR("Failed to allocate SoundState");
        return -1;
    }
    soundState->transientStorage = &transientStorage;
    soundState->allocatedsoundsBuffer = bump_alloc(&persistentStorage, SOUNDS_BUFFER_SIZE);
    if (!soundState->allocatedsoundsBuffer)
    {
        LOG_ERROR("Failed to allocated sounds buffer");
        return -1;
    }

    platform_fill_keycode_lookup_table();
    platform_create_window(1280, 720, "Vaults Below");
    LOG_INFO("creating window");

    if (!platform_init_audio())
    {
        LOG_ERROR("Failed to initialize audio");
        return -1;
    }
    else
    {
        LOG_INFO("audio has been initialized");
    }

    gl_init(&transientStorage);

    while (running)
    {
        dt = get_delta_time();
        reload_game_dll(&transientStorage);

        // update
        platform_update_window();
        update_game(gameState, renderData, input, soundState, dt);
        gl_render(&transientStorage);
        platform_update_audio(dt);

        platform_swap_buffers();

        transientStorage.used = 0;
    }

    return 0;
}

void update_game(GameState *gameStateIn, RenderData *renderDataIn, Input *inputIn, SoundState *soundStateIn, float dt)
{
    update_game_ptr(gameStateIn, renderDataIn, inputIn, soundStateIn, dt);
}

double get_delta_time()
{
    // Only executed once when entering the function (static)
    static auto lastTime = std::chrono::steady_clock::now();
    auto currentTime = std::chrono::steady_clock::now();

    // seconds
    double delta = std::chrono::duration<double>(currentTime - lastTime).count();
    lastTime = currentTime;

    return delta;
}

void reload_game_dll(BumpAllocator *transientStorage)
{
    static void *gameDLL;
    static long long lastEditTimestampGameDLL;

    long long currentTimestampGameDLL = get_timestamp("game.dll");
    if (currentTimestampGameDLL > lastEditTimestampGameDLL)
    {
        if (gameDLL)
        {
            bool freeResult = platform_free_dynamic_library(gameDLL);
            LOG_ASSERT(freeResult, "Failed to free game.dll");
            gameDLL = nullptr;
            LOG_TRACE("Freed game.dll");
        }

        while (!copy_file("game.dll", "game_load.dll", transientStorage))
        {
            Sleep(10);
        }
        LOG_TRACE("Copied game.dll into game_load.dll");

        gameDLL = platform_load_dynamic_library("game_load.dll");
        LOG_ASSERT(gameDLL, "Failed to load game_load.dll");

        update_game_ptr = (update_game_type *)platform_load_dynamic_function(gameDLL, "update_game");
        LOG_ASSERT(update_game_ptr, "Failed to load update_game function");
        lastEditTimestampGameDLL = currentTimestampGameDLL;
    }
}
