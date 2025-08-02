#include "vaultEngine_lib.h"
#include "input.h"
#include "platform.h"

#define APIENTRY
#define GL_GLEXT_PROTOTYPES
#include"glcorearb.h"

#ifdef _WIN32
#include"win32_platform.cpp"
#endif

#include "gl_renderer.cpp"

int main()
{
    BumpAllocator transientStorage = makeBumpAllocator(MB(50));

    input.screenSizeX = 1200;
    input.screenSizeY = 720;
    platformCreateWindow(input.screenSizeX, input.screenSizeY, "Vaults Below");

    gl_init(&transientStorage);

    while (running)
    {
        // update
        platformUpdateWindow();
        gl_render();

        platform_swap_buffer();
    }

    return 0;
}