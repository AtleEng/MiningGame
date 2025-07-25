#pragma once

#include <stdio.h>

// ################################     Defines    ################################

// ################################     Logging    ################################
enum TextColor
{
    textColorBlack,
    textColorRed,
    textColorGreen,
    textColorYellow,
    textColorBlue,
    textColorMagenta,
    textColorCyan,
    textColorWhite,
    textColorBrightBlack,
    textColorBrightRed,
    textColorBrightGreen,
    textColorBrightYellow,
    textColorBrightBlue,
    textColorBrightMagenta,
    textColorBrightCyan,
    textColorBrightWhite,
    textColorCount
};
template <typename ...Args>
void _log(char* prefix, char* msg, TextColor textColor, Args... args)
{
    static char* TextColorTable[textColorCount] = 
  {    
    "\x1b[30m", // BLACK
    "\x1b[31m", // RED
    "\x1b[32m", // GREEN
    "\x1b[33m", // YELLOW
    "\x1b[34m", // BLUE
    "\x1b[35m", // MAGENTA
    "\x1b[36m", // CYAN
    "\x1b[37m", // WHITE
    "\x1b[90m", // BRIGHT BLACK
    "\x1b[91m", // BRIGHT RED
    "\x1b[92m", // BRIGHT GREEN
    "\x1b[93m", // BRIGHT YELLOW
    "\x1b[94m", // BRIGHT BLUE
    "\x1b[95m", // BRIGHT MAGENTA
    "\x1b[96m", // BRIGHT CYAN
    "\x1b[97m", // BRIGHT WHITE
  };

  char formatBuffer[8192] = {};
  sprintf(formatBuffer, "%s %s %s \033[0m", TextColorTable[textColor], prefix, msg);

  char textBuffer[8912] = {};
  sprintf(textBuffer, formatBuffer, args...);

  puts(textBuffer);
}


#define SM_TRACE(msg, ...) _log("TRACE: ", msg, textColorGreen, ##__VA_ARGS__);
#define SM_WARN(msg, ...) _log("TRACE: ", msg, textColorYellow, ##__VA_ARGS__);
#define SM_ERROR(msg, ...) _log("TRACE: ", msg, textColorRed, ##__VA_ARGS__);