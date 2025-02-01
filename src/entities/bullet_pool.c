#include "entities/bullet_pool.h"
#include "entities/bullet.h"

void bullet_pool_init(BulletPool *pool) {
    for (int i = 0; i < BULLET_POOL_CAPACITY; i++) {
        pool->bullets[i] = NULL;
    }
}

void bullet_pool_spawn(BulletPool *pool, float x, float y, float vx, float vy) {
    for (int i = 0; i < BULLET_POOL_CAPACITY; i++) {
        if (pool->bullets[i] == NULL || !pool->bullets[i]->alive) {
            if (pool->bullets[i] == NULL) {
                pool->bullets[i] = entity_create(ENTITY_BULLET, x, y);
                pool->bullets[i]->update = bullet_update;
            } else {
                pool->bullets[i]->pos.x = x;
                pool->bullets[i]->pos.y = y;
                pool->bullets[i]->alive = 1;
            }
            pool->bullets[i]->vel.x = vx;
            pool->bullets[i]->vel.y = vy;
            break;
        }
    }
}

void bullet_pool_update(BulletPool *pool, float dt) {
    for (int i = 0; i < BULLET_POOL_CAPACITY; i++) {
        if (pool->bullets[i] && pool->bullets[i]->alive && pool->bullets[i]->update)
            pool->bullets[i]->update(pool->bullets[i], dt);
    }
}



void bullet_pool_destroy(BulletPool *pool) {
    for (int i = 0; i < BULLET_POOL_CAPACITY; i++) {
        if (pool->bullets[i]) {
            entity_destroy(pool->bullets[i]);
            pool->bullets[i] = NULL;
        }
    }
}
