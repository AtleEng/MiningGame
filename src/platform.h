#pragma once

// ################################     Platform Globals    ################################
static bool running = true;

// ################################     Platform Functions    ################################
bool platformCreateWindow(int width, int height, char *title);
void platformUpdateWindow();
void* platform_load_gl_function(char* funcName);