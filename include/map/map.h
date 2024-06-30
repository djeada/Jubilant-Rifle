#ifndef MAP_H
#define MAP_H
#include <stdlib.h>

typedef struct {
  int x;
  int y;
  int width;
  int height;
} Platform;

typedef struct {
  Platform *platforms;
  size_t platformCount;
  char *backgroundImage;
    int width;
  int height;
} Map;

int allocatePlatforms(Map* map, size_t count);
char* allocateBackgroundImagePath(const char* path);
int parseStringValue(const char* json, const char* key, char* value, size_t valueSize);
int parseIntValue(const char* json, const char* key, int* value);
int parsePlatforms(const char* json, Platform* platforms, size_t platformCount);
int parseMapFile(const char* filePath, Map* map);
void mapDestructor(Map* map);

#endif
