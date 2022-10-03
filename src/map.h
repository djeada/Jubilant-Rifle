#ifndef MAP_H
#define MAP_H

#include "utils.h"

typedef struct {
  int startX;
  int endX;
  int y;
} Platform;

typedef struct {
  int startHeight;
  int endHeight;
  int numberOfPlatforms;
  Platform *platforms;
} Level;

typedef struct {
  int numberOfLevels;
  Level *levels;
} Map;

int coordinatesToGroundLevel(Map *map, Point *position);
void printMap(Map *map);
void freeMap(Map *map);

#endif
