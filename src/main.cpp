#include "vaultEngine_lib.h"
// ################################     Platform Globals    ################################
static bool running = true;

// ################################     Platform Functions    ################################
bool platformCreateWindow(int width, int height, char *title);
void platformUpdateWindow();

// ################################     Windows Platform    ################################
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

// ################################     Windows Globals    ################################
static HWND window;

// ################################     Platform Implementations    ################################
LRESULT CALLBACK windowsWindowCallback(HWND window, UINT msg,
                                          WPARAM wParam, LPARAM lparam)
{
    LRESULT result = 0;

    switch (msg)
    {
    case WM_CLOSE:
        running = false;
        break;

    default:
        result = DefWindowProcA(window, msg, wParam, lparam);
    }
    return result;
}

bool platformCreateWindow(int width, int height, char *title)
{
    HINSTANCE instance = GetModuleHandleA(0);

    WNDCLASSA wc = {};
    wc.hInstance = instance;
    wc.hIcon = LoadIcon(instance, IDI_APPLICATION); // defult Icon
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);       // defult cursor
    wc.lpszClassName = title;                       // unique identifier for the window (now uses the title)
    wc.lpfnWndProc = windowsWindowCallback;      // Callback for input to window

    if (!RegisterClassA(&wc))
    {
        return false;
    }
    // windows style
    int dwStyle = WS_OVERLAPPEDWINDOW;

    window = CreateWindowExA(0, title, // reference: lpszClassName
                             title,    // title of window
                             dwStyle,  // style
                             100,      // Pos x of window
                             100,      // Pos y of window
                             width,    // width of window
                             height,   // height of window
                             NULL,     // parent
                             NULL,     // menu
                             instance,
                             NULL); // lpParam
    if (window == NULL)
    {
        return false;
    }

    ShowWindow(window, SW_SHOW);

    return true;
}

void platformUpdateWindow()
{
    MSG msg;

    while (PeekMessageA(&msg, window, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

#endif

int main()
{
    platformCreateWindow(1200, 720, "Vaults Below");

    while (running)
    {
        // update
        platformUpdateWindow();

        SM_TRACE("Test");
        SM_WARN("Test");
        SM_ERROR("Test");
    }

    return 0;
}