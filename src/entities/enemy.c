#include "entities/enemy.h"

void enemy_update(Entity *self, float dt) {
  Enemy *enemy = (Enemy *)self;
  common_update(self, dt);
  enemy->shootTimer -= dt;
}