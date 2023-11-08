#ifndef BULLET_H
#define BULLET_H

#include "utils/utils.h"

#include <stdbool.h>

typedef struct {
  Point position;
  float dx;
} Bullet;

void bulletConstructor(Bullet *bullet, Point position, float dx);
void bulletCopyConstructor(Bullet *destination, Bullet *source);
void moveBullet(Bullet *bullet);
bool bulletOutOfScreen(Bullet *bullet);

#endif
