#include "vaultEngine_lib.h"
#include "platform.h"

#define APIENTRY
#define GL_GLEXT_PROTOTYPES
#include"glcorearb.h"

#ifdef _WIN32
#include"win32_platform.cpp"
#endif

#include "gl_Renderer.cpp"

int main()
{
    BumpAllocator transientStorage = makeBumpAllocator(MB(50));

    platformCreateWindow(1200, 720, "Vaults Below");

    gl_init(&transientStorage);

    while (running)
    {
        // update
        platformUpdateWindow();

    }

    return 0;
}