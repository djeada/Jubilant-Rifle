#ifndef ENEMY_H
#define ENEMY_H

#include "entities/humanoid.h"

typedef struct {
  Humanoid base;
  // Enemy specific fields...
  Point patrolStart;
  Point patrolEnd;
} Enemy;

#endif
