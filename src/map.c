#include "map.h"
#include "consts.h"

/*
int coordinatesToGroundLevel(int x, int y) {
  if (y < 160) {
    if (x > 80 && x < 300) {
      return 140;
    }
    if (x > 360 && x < 510 && y < 120) {
      return 105;
    }
  }

  if (y > 150 && y < 300) {
    if (x >= 140 && x < 355) {
      return 250;
    }
  }

  if (y > 290 && y < 330) {
    if (x < 180) {
      return 305;
    }

    if (x >= 380) {
      return 295;
    }
  }

  return DEATH_LEVEL;
}*/



unsigned int coordinatesToGroundLevel(Map* map, Point* position) {
  for (unsigned int i = 0; i < map->numberOfLevels; i++) {
    Level* level = &(map->levels[i]);
    if (position->y > level->startHeight && position->y < level->endHeight) {
      for (unsigned int j = 0; j < level->numberOfPlatforms; j++) {
        Platform* platform = &(level->platforms[j]);
        if (position->x >= platform->startX && position->x <= platform->endX) {
          return platform->y;
        }
      }
    }
  }

  return DEATH_LEVEL;
}

