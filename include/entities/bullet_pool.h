#ifndef BULLET_POOL_H
#define BULLET_POOL_H

#include "entities/entity.h"
#include <SDL2/SDL.h>
#include <stdlib.h>

typedef struct {
    Entity **bullets;      // Array of bullet pointers.
    int capacity;          // Total capacity of the pool.

    int *activeIndices;    // Array of indices into 'bullets' that are currently active.
    int activeCount;       // The number of active bullets.

    int *freeList;         // Free list: a linked list (by indices) of available bullet slots.
    int freeListHead;      // Head index of the free list.
} BulletPool;

void bulletPoolInit(BulletPool *pool, int capacity);
void bulletPoolSpawn(BulletPool *pool, float x, float y, float vx, float vy);
void bulletPoolUpdate(BulletPool *pool, float dt);
void bulletPoolDestroy(BulletPool *pool);

#endif // BULLET_POOL_H
