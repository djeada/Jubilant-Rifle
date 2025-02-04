#include "entities/enemy.h"
#include <stdlib.h>

Enemy *enemyCreate(float x, float y) {
  Enemy *enemy = malloc(sizeof(Enemy));
  if (!enemy)
    return NULL;

  enemy->base = *entityCreate(ENTITY_ENEMY, x, y);
  enemy->base.health = 30;
  enemy->base.update = enemyUpdate;
  enemy->shootTimer = 2.0f;
  enemy->base.anim = malloc(sizeof(Animation));
  enemy->base.anim->frameCount = 6;
  enemy->base.anim->frameDuration = 0.2f;
  enemy->base.anim->timer = 0;
  enemy->base.anim->currentFrame = 0;
  enemy->base.direction = DIRECTION_LEFT;

  return enemy;
}

void enemyUpdate(Entity *self, float dt) {
  Enemy *enemy = (Enemy *)self;
  commonEntityUpdate(self, dt);
  enemy->shootTimer -= dt;
}

bool isEnemyAlive(const Enemy *enemy) { return isEntityAlive(&enemy->base); }

Direction getEnemyFacingDirection(const Enemy *enemy) {
  return enemy->base.direction;
}

bool isEnemyMoving(const Enemy *enemy) { return isEntityMoving(&enemy->base); }

void enemySetHorizontalVelocity(Enemy *enemy, float vx) {
  if (vx > 0)
    enemy->base.direction = DIRECTION_RIGHT;
  else if (vx < 0)
    enemy->base.direction = DIRECTION_LEFT;
  enemy->base.vel.x = vx;
}

void enemySetVerticalVelocity(Enemy *enemy, float vy) {
  enemy->base.vel.y = vy;
}

void enemyStopHorizontal(Enemy *enemy) { enemySetHorizontalVelocity(enemy, 0); }

void enemyStopVertical(Enemy *enemy) { enemySetVerticalVelocity(enemy, 0); }

void enemyShoot(Enemy *enemy, BulletPool *bulletPool) {
  bulletPoolSpawn(bulletPool, BULLET_SOURCE_ENEMY, enemy->base.pos.x + (50 / 2),
                  enemy->base.pos.y, 300, 0);
}

