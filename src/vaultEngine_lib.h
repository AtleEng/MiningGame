#pragma once

#include <stdio.h>
#include <stdlib.h> //malloc
#include <string.h> //memset
#include <sys/stat.h> //edit timestamp of files

// ################################     Defines    ################################
#ifdef _WIN32
#define DEBUG_BREAK() __debugbreak()
#define EXPORT_FN __declspec(dllexport)
#elif __linux__
#define DEBUG_BREAK() __builtin_debugtrap()
#elif __APPLE__
#define DEBUG_BREAK() __builtin_trap()
#endif

#define BIT(x) 1 << (x)
#define KB(x) ((unsigned long long)1024 * x)
#define MB(x) ((unsigned long long)1024 * KB(x))
#define GB(x) ((unsigned long long)1024 * MB(x))
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
template <typename... Args>
void _log(char *prefix, char *msg, TextColor textColor, Args... args)
{
  static char *TextColorTable[textColorCount] =
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

#define SM_LOG(msg, textColor, ...) _log("LOG: ", msg, textColor, ##__VA_ARGS__);
#define SM_TRACE(msg, ...) _log("TRACE: ", msg, textColorGreen, ##__VA_ARGS__);
#define SM_WARN(msg, ...) _log("WARN: ", msg, textColorYellow, ##__VA_ARGS__);
#define SM_ERROR(msg, ...) _log("ERROR: ", msg, textColorRed, ##__VA_ARGS__);

#define SM_ASSERT(x, msg, ...)      \
  {                                 \
    if (!(x))                       \
    {                               \
      SM_ERROR(msg, ##__VA_ARGS__); \
      DEBUG_BREAK();                \
      SM_ERROR("ASSERTION HIT");    \
    }                               \
  }

// ################################     BumpAllocator    ################################
struct BumpAllocator
{
  size_t capacity;
  size_t used;
  char *memory;
};

BumpAllocator make_bump_allocator(size_t size)
{
  BumpAllocator ba = {};

  ba.memory = (char *)malloc(size);
  if (ba.memory)
  {
    ba.capacity = size;
    memset(ba.memory, 0, size); // Set memory to 0
  }
  else
  {
    SM_ASSERT(false, "Failed to allocate memory")
  }
  return ba;
}

char *bump_alloc(BumpAllocator *bumpAllocator, size_t size)
{
  char *result = nullptr;

  size_t allignedSize = (size + 7) & ~7; // Make sure the first 4 bits are 0
  if (bumpAllocator->used + allignedSize <= bumpAllocator->capacity)
  {
    result = bumpAllocator->memory + bumpAllocator->used;
    bumpAllocator->used += allignedSize;
  }
  else
  {
    SM_ASSERT(false, "BumpAllocator is full");
  }
  return result;
}

// ################################     File I/O    ################################
long long get_timestamp(const char* file)
{
  struct stat file_stat = {};
  stat(file, &file_stat);
  return file_stat.st_mtime;
}

bool file_exists(const char* filePath)
{
  SM_ASSERT(filePath, "No filePath supplied!");

  auto file = fopen(filePath, "rb");
  if(!file)
  {
    return false;
  }
  fclose(file);

  return true;
}

long get_file_size(const char* filePath)
{
  SM_ASSERT(filePath, "No filePath supplied!");

  long fileSize = 0;
  auto file = fopen(filePath, "rb");
  if(!file)
  {
    SM_ERROR("Failed opening File: %s", filePath);
    return 0;
  }

  fseek(file, 0, SEEK_END);
  fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);
  fclose(file);

  return fileSize;
}

/*
* Reads a file into a supplied buffer. We manage our own
* memory and therefore want more control over where it 
* is allocated
*/
char* read_file(const char* filePath, int* fileSize, char* buffer)
{
  SM_ASSERT(filePath, "No filePath supplied!");
  SM_ASSERT(fileSize, "No fileSize supplied!");
  SM_ASSERT(buffer, "No buffer supplied!");

  *fileSize = 0;
  auto file = fopen(filePath, "rb");
  if(!file)
  {
    SM_ERROR("Failed opening File: %s", filePath);
    return nullptr;
  }

  fseek(file, 0, SEEK_END);
  *fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);

  memset(buffer, 0, *fileSize + 1); //+1 to make sure the file is terminated
  fread(buffer, sizeof(char), *fileSize, file);

  fclose(file);

  return buffer;
}

char* read_file(const char* filePath, int* fileSize, BumpAllocator* bumpAllocator)
{
  char* file = nullptr;
  long fileSize2 = get_file_size(filePath);

  if(fileSize2)
  {
    char* buffer = bump_alloc(bumpAllocator, fileSize2 + 1);

    file = read_file(filePath, fileSize, buffer);
  }

  return file; 
}

void write_file(const char* filePath, char* buffer, int size)
{
  SM_ASSERT(filePath, "No filePath supplied!");
  SM_ASSERT(buffer, "No buffer supplied!");
  auto file = fopen(filePath, "wb");
  if(!file)
  {
    SM_ERROR("Failed opening File: %s", filePath);
    return;
  }

  fwrite(buffer, sizeof(char), size, file);
  fclose(file);
}

bool copy_file(const char* fileName, const char* outputName, char* buffer)
{
  int fileSize = 0;
  char* data = read_file(fileName, &fileSize, buffer);

  auto outputFile = fopen(outputName, "wb");
  if(!outputFile)
  {
    SM_ERROR("Failed opening File: %s", outputName);
    return false;
  }

  int result = fwrite(data, sizeof(char), fileSize, outputFile);
  if(!result)
  {
    SM_ERROR("Failed opening File: %s", outputName);
    return false;
  }
  
  fclose(outputFile);

  return true;
}

bool copy_file(const char* fileName, const char* outputName, BumpAllocator* bumpAllocator)
{
  char* file = 0;
  long fileSize2 = get_file_size(fileName);

  if(fileSize2)
  {
    char* buffer = bump_alloc(bumpAllocator, fileSize2 + 1);

    return copy_file(fileName, outputName, buffer);
  }

  return false;
}

// ################################     Math    ################################

struct Vec2
{
  float x;
  float y;
};
struct IVec2
{
  int x;
  int y;
};
