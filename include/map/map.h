#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include <SDL2/SDL.h>
#include "utils/point.h"

// Use SDL_Rect for platforms.
typedef SDL_Rect Platform;

typedef struct {
    Platform *platforms;
    size_t platformCount;
    char *backgroundImage;
    SDL_Rect rect; // The map's boundaries: top-left is (0, 0), bottom is (width, height).
} Map;

Point getRandomPositionOnPlatform(Platform* platform);
int allocatePlatforms(Map* map, size_t count);
char* allocateBackgroundImagePath(const char* path);
int parseStringValue(const char* json, const char* key, char* value, size_t valueSize);
int parseIntValue(const char* json, const char* key, int* value);
int parsePlatforms(const char* json, Platform* platforms, size_t platformCount);
int parseMapFile(const char* filePath, Map* map);
void mapDestructor(Map* map);

#endif
