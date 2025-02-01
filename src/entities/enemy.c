#include "entities/enemy.h"

Enemy *enemy_create(float x, float y) {
  Enemy *enemy = malloc(sizeof(Enemy));
  enemy->base = *entity_create(ENTITY_ENEMY, x, y);
  enemy->base.health = 30;
  enemy->base.update = enemy_update;
  enemy->shootTimer = 2.0f;
  enemy->base.anim = malloc(sizeof(Animation));
  enemy->base.anim->frameCount = 6;       // adjust if needed
  enemy->base.anim->frameDuration = 0.2f; // slower animation, for example
  enemy->base.anim->timer = 0;
  enemy->base.anim->currentFrame = 0;
  return enemy;
}

void enemy_update(Entity *self, float dt) {
  Enemy *enemy = (Enemy *)self;
  common_update(self, dt);
  enemy->shootTimer -= dt;
}
