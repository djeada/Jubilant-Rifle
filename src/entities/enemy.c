#include "entities/enemy.h"

Enemy *enemyCreate(float x, float y) {
  Enemy *enemy = malloc(sizeof(Enemy));
  enemy->base = *entityCreate(ENTITY_ENEMY, x, y);
  enemy->base.health = 30;
  enemy->base.update = enemyUpdate;
  enemy->shootTimer = 2.0f;
  enemy->base.anim = malloc(sizeof(Animation));
  enemy->base.anim->frameCount = 6;       // adjust if needed
  enemy->base.anim->frameDuration = 0.2f; // slower animation, for example
  enemy->base.anim->timer = 0;
  enemy->base.anim->currentFrame = 0;
  return enemy;
}

void enemyUpdate(Entity *self, float dt) {
  Enemy *enemy = (Enemy *)self;
  commonEntityUpdate(self, dt);
  enemy->shootTimer -= dt;
}
