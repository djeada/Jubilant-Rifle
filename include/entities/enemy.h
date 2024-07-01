#ifndef ENEMY_H
#define ENEMY_H

#include "entities/humanoid.h"

typedef struct {
  Humanoid base;
  Point patrolStart;
  Point patrolEnd;
} Enemy;

bool enemyIsAlive(Enemy *enemy);

#endif
