#include "bullet.h"
#include <stdlib.h>

void bulletConstructor(Bullet *bullet, Point position, float dx) {
  pointCopyConstructor(&bullet->position, &position);
  bullet->dx = dx;
}

void bulletCopyConstructor(Bullet *destination, Bullet *source) {
  bulletConstructor(destination, source->position, source->dx);
}
