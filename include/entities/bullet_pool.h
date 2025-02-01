#ifndef BULLET_POOL_H
#define BULLET_POOL_H

#include "entities/entity.h"
#include <SDL2/SDL.h>

#define BULLET_POOL_CAPACITY 100

typedef struct {
    Entity *bullets[BULLET_POOL_CAPACITY];
} BulletPool;

void bulletPoolInit(BulletPool *pool);
void bulletPoolSpawn(BulletPool *pool, float x, float y, float vx, float vy);
void bulletPoolUpdate(BulletPool *pool, float dt);
void bulletPoolDestroy(BulletPool *pool);

#endif // BULLET_POOL_H
