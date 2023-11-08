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
} Map;

int parseMapFile(const char *filePath, Map *map);
void freeMapResources(Map *map);
#endif
