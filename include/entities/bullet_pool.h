#ifndef BULLET_POOL_H
#define BULLET_POOL_H

#include "entities/entity.h"
#include <SDL2/SDL.h>

#define BULLET_POOL_CAPACITY 100

typedef struct {
    Entity *bullets[BULLET_POOL_CAPACITY];
} BulletPool;

void bullet_pool_init(BulletPool *pool);
void bullet_pool_spawn(BulletPool *pool, float x, float y, float vx, float vy);
void bullet_pool_update(BulletPool *pool, float dt);
void bullet_pool_destroy(BulletPool *pool);

#endif // BULLET_POOL_H
