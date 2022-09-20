#include "bullet.h"
#include <stdlib.h>

void bulletConstructor(Bullet *bullet, float x, float y, float dx) {
  bullet->x = x;
  bullet->y = y;
  bullet->dx = dx;
}
