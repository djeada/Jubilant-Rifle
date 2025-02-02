#ifndef BULLET_POOL_H
#define BULLET_POOL_H

#include "entities/bullet.h"
#include <SDL2/SDL.h>
#include <stdlib.h>

/*
 * The BulletPool now holds two separate sub-pools:
 * one for player bullets and one for enemy bullets.
 */
typedef struct {
    // --- Player bullet sub-pool ---
    Bullet **playerBullets;       // Array of pointers to player bullets.
    int playerCapacity;           // Capacity for player bullets.
    int *playerActiveIndices;     // Indices of currently active player bullets.
    int playerActiveCount;        // Number of active player bullets.
    int *playerFreeList;          // Linked list (by indices) of free player slots.
    int playerFreeListHead;       // Head index of the free list.

    // --- Enemy bullet sub-pool ---
    Bullet **enemyBullets;        // Array of pointers to enemy bullets.
    int enemyCapacity;            // Capacity for enemy bullets.
    int *enemyActiveIndices;      // Indices of currently active enemy bullets.
    int enemyActiveCount;         // Number of active enemy bullets.
    int *enemyFreeList;           // Linked list (by indices) of free enemy slots.
    int enemyFreeListHead;        // Head index of the free list.
} BulletPool;

void bulletPoolInit(BulletPool *pool, int totalCapacity);
void bulletPoolSpawn(BulletPool *pool, BulletSource source, float x, float y, float vx, float vy);
void bulletPoolUpdate(BulletPool *pool, float dt);
void bulletPoolDestroy(BulletPool *pool);

#endif // BULLET_POOL_H