#include "map.h"
#include "consts.h"

unsigned int coordinatesToGroundLevel(Map *map, Point *position) {
  for (unsigned int i = 0; i < map->numberOfLevels; i++) {
    Level *level = &(map->levels[i]);
    if (position->y > level->startHeight && position->y < level->endHeight) {
      for (unsigned int j = 0; j < level->numberOfPlatforms; j++) {
        Platform *platform = &(level->platforms[j]);
        if (position->x >= platform->startX && position->x <= platform->endX) {
          return platform->y;
        }
      }
    }
  }

  return DEATH_LEVEL;
}

void printMap(Map *map) {
  printf("Number of levels: %d \n", map->numberOfLevels);
  for (unsigned int i = 0; i < map->numberOfLevels; i++) {
    Level *level = &(map->levels[i]);
    printf("Level %d: \n", i);
    printf("Start height: %d \n", level->startHeight);
    printf("End height: %d \n", level->endHeight);
    printf("Number of platforms: %d \n", level->numberOfPlatforms);
    for (unsigned int j = 0; j < level->numberOfPlatforms; j++) {
      Platform *platform = &(level->platforms[j]);
      printf("Platform %d: \n", j);
      printf("Start X: %d \n", platform->startX);
      printf("End X: %d \n", platform->endX);
      printf("Y: %d \n", platform->y);
    }
  }
}

void freeMap(Map *map) {
  for (unsigned int i = 0; i < map->numberOfLevels; i++) {
    Level *level = &(map->levels[i]);
    free(level->platforms);
  }
  free(map->levels);
}
