#include <stddef.h>
#include <stdlib.h>

#include "bullet.h"
#include "consts.h"

void addBullet(float x, float y, float dx, Bullet **bullets) {
  int found = -1;
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (bullets[i] == NULL) {
      found = i;
      break;
    }
  }

  if (found >= 0) {
    int i = found;
    bullets[i] = malloc(sizeof(Bullet));
    bullets[i]->x = x;
    bullets[i]->y = y;
    bullets[i]->dx = dx;
  }
}

void removeBullet(int i, Bullet **bullets) {
  if (bullets[i]) {
    free(bullets[i]);
    bullets[i] = NULL;
  }
}

void cleanupBullets(Bullet **bullets) {
  for (int i = 0; i < MAX_BULLETS; i++)
    removeBullet(i, bullets);
}