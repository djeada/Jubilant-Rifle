#include "bullet.h"
#include <stdlib.h>

void bulletConstructor(Bullet *bullet, float x, float y, float dx) {
  bullet->x = x;
  bullet->y = y;
  bullet->dx = dx;
}

void bulletCopyConstructor(Bullet *destination, Bullet *source) {
  bulletConstructor(destination, source->x, source->y, source->dx);
}
