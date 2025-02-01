#include "entities/bullet_pool.h"
#include "entities/bullet.h"

// TODO:
// - we can probably get disregard health for bullets

void bulletPoolInit(BulletPool *pool) {
  for (int i = 0; i < BULLET_POOL_CAPACITY; i++) {
    pool->bullets[i] = NULL;
  }
}

void bulletPoolSpawn(BulletPool *pool, float x, float y, float vx, float vy) {
  for (int i = 0; i < BULLET_POOL_CAPACITY; i++) {
    if (pool->bullets[i] == NULL || pool->bullets[i]->health < 0) {
      if (pool->bullets[i] == NULL) {
        pool->bullets[i] = entityCreate(ENTITY_BULLET, x, y);
        pool->bullets[i]->update = bulletUpdate;
      } else {
        pool->bullets[i]->pos.x = x;
        pool->bullets[i]->pos.y = y;
        pool->bullets[i]->health = 1;
      }
      pool->bullets[i]->vel.x = vx;
      pool->bullets[i]->vel.y = vy;
      break;
    }
  }
}

void bulletPoolUpdate(BulletPool *pool, float dt) {
  for (int i = 0; i < BULLET_POOL_CAPACITY; i++) {
    if (pool->bullets[i] && pool->bullets[i]->health > 0 &&
        pool->bullets[i]->update)
      pool->bullets[i]->update(pool->bullets[i], dt);
  }
}

void bulletPoolDestroy(BulletPool *pool) {
  for (int i = 0; i < BULLET_POOL_CAPACITY; i++) {
    if (pool->bullets[i]) {
      entityDestroy(pool->bullets[i]);
      pool->bullets[i] = NULL;
    }
  }
}
