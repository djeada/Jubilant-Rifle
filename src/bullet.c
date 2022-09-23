#include "bullet.h"
#include "consts.h"

void bulletConstructor(Bullet *bullet, Point position, float dx) {
  pointCopyConstructor(&bullet->position, &position);
  bullet->dx = dx;
}

void bulletCopyConstructor(Bullet *destination, Bullet *source) {
  bulletConstructor(destination, source->position, source->dx);
}

void moveBullet(Bullet *bullet) { bullet->position.x += bullet->dx; }

bool bulletOutOfScreen(Bullet *bullet) {
  return bullet->position.x < -0.1 * BOARD_WIDTH ||
         bullet->position.x > 1.1 * BOARD_WIDTH;
}
