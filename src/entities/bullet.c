#include "entities/bullet.h"

void bulletUpdate(Entity *self, float dt) {
  commonEntityUpdate(self, dt);
  if (self->pos.x < 0 || self->pos.x > 640 || self->pos.y < 0 ||
      self->pos.y > 480)
    self->health = 0;
}
