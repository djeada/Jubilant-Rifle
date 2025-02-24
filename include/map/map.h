#ifndef MAP_H
#define MAP_H

#include "utils/point.h"
#include <SDL2/SDL.h>
#include <stdlib.h>

// Use SDL_Rect for platforms.
typedef SDL_Rect Platform;

typedef struct {
    Platform *platforms;
    size_t platformCount;
    char *backgroundImage;
    SDL_Rect rect; // Map boundaries: top-left is (0,0), dimensions from JSON.
} Map;

Point getRandomPositionOnPlatform(const Platform *platform);
int allocatePlatforms(Map *map, size_t count);
char *allocateBackgroundImagePath(const char *path);
int parseStringValue(const char *json, const char *key, char *value, size_t valueSize);
int parseIntValue(const char *json, const char *key, int *value);
int parsePlatforms(const char *json, Platform *platforms, size_t platformCount);
int parseMapFile(const char *filePath, Map *map);
void mapDestructor(Map *map);

#endif // MAP_H
