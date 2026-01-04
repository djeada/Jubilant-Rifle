#include "entities/enemy.h"
#include <stdlib.h>

#define PARACHUTE_SPEED 50.0f
#define LANDING_DURATION 0.5f
#define ENEMY_GRENADE_COOLDOWN 5.0f
#define ENEMY_INITIAL_GRENADES 2

static void initEnemyCommon(Enemy *enemy) {
  enemy->state = ENEMY_STATE_NORMAL;
  enemy->parachuteSpeed = PARACHUTE_SPEED;
  enemy->stateTimer = 0.0f;
  enemy->grenadeCount = 0;
  enemy->grenadeTimer = 0.0f;
}

Enemy *enemyCreate(float x, float y) {
  Enemy *enemy = malloc(sizeof(Enemy));
  if (!enemy)
    return NULL;

  /* Create temporary entity to copy values from */
  Entity *tempEntity = entityCreate(ENTITY_ENEMY, x, y);
  if (!tempEntity) {
    free(enemy);
    return NULL;
  }
  enemy->base = *tempEntity;
  free(tempEntity); /* Free the temporary entity, values are copied */

  enemy->base.health = 30;
  enemy->base.update = enemyUpdate;
  enemy->shootTimer = 2.0f;
  enemy->base.anim = malloc(sizeof(Animation));
  if (!enemy->base.anim) {
    free(enemy);
    return NULL;
  }
  enemy->base.anim->tex = NULL;
  enemy->base.anim->frameCount = 6;
  enemy->base.anim->frameDuration = 0.2f;
  enemy->base.anim->timer = 0;
  enemy->base.anim->currentFrame = 0;
  enemy->base.direction = DIRECTION_LEFT;
  
  initEnemyCommon(enemy);

  return enemy;
}

Enemy *enemyCreateParachuting(float x, float y, float targetY) {
  Enemy *enemy = enemyCreate(x, y);
  if (!enemy)
    return NULL;
  
  enemy->state = ENEMY_STATE_PARACHUTING;
  enemy->base.vel.y = enemy->parachuteSpeed;
  enemy->stateTimer = (targetY - y) / enemy->parachuteSpeed;
  enemy->grenadeCount = ENEMY_INITIAL_GRENADES;
  
  return enemy;
}

void enemyDestroy(Enemy *enemy) {
  if (enemy) {
    if (enemy->base.anim) {
      free(enemy->base.anim);
    }
    free(enemy);
  }
}

void enemyUpdate(Entity *self, float dt) {
  Enemy *enemy = (Enemy *)self;
  
  /* Handle parachuting state */
  if (enemy->state == ENEMY_STATE_PARACHUTING) {
    enemy->stateTimer -= dt;
    if (enemy->stateTimer <= 0) {
      enemy->state = ENEMY_STATE_LANDING;
      enemy->stateTimer = LANDING_DURATION;
      enemy->base.vel.y = 0;
    }
  } else if (enemy->state == ENEMY_STATE_LANDING) {
    enemy->stateTimer -= dt;
    if (enemy->stateTimer <= 0) {
      enemy->state = ENEMY_STATE_NORMAL;
    }
  }
  
  /* Update grenade cooldown */
  if (enemy->grenadeTimer > 0) {
    enemy->grenadeTimer -= dt;
  }
  
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

bool isEnemyParachuting(const Enemy *enemy) {
  if (!enemy)
    return false;
  return enemy->state == ENEMY_STATE_PARACHUTING;
}

void enemyAddGrenades(Enemy *enemy, int count) {
  if (!enemy || count <= 0)
    return;
  enemy->grenadeCount += count;
}

bool enemyCanThrowGrenade(const Enemy *enemy) {
  if (!enemy)
    return false;
  return enemy->grenadeCount > 0 && enemy->grenadeTimer <= 0 &&
         enemy->state == ENEMY_STATE_NORMAL;
}

bool enemyUseGrenade(Enemy *enemy) {
  if (!enemyCanThrowGrenade(enemy))
    return false;
  enemy->grenadeCount--;
  enemy->grenadeTimer = ENEMY_GRENADE_COOLDOWN;
  return true;
}

