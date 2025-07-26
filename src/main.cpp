#include "vaultEngine_lib.h"
#include "platform.h"

#define APIENTRY
#include"glcorearb.h"

#ifdef _WIN32
#include"win32_platform.cpp"
#endif

#include "gl_Renderer.h"

int main()
{
    platformCreateWindow(1200, 720, "Vaults Below");

    while (running)
    {
        // update
        platformUpdateWindow();

    }

    return 0;
}