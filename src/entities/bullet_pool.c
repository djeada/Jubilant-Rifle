#include "entities/bullet_pool.h"
#include "utils/consts.h"
#include <stdlib.h>

/*
 * Initialize the bullet pool.
 * The pool is divided into two sub-pools: one for player bullets and one for
 * enemy bullets.
 */
void bulletPoolInit(BulletPool *pool, int totalCapacity) {
  // Split capacity: player gets half, enemy gets the rest.
  int playerCap = totalCapacity / 2;
  int enemyCap = totalCapacity - playerCap;

  /* --- Initialize Player Bullet Sub-Pool --- */
  pool->playerCapacity = playerCap;
  pool->playerActiveCount = 0;
  pool->playerBullets = malloc(sizeof(Bullet *) * playerCap);
  pool->playerActiveIndices = malloc(sizeof(int) * playerCap);
  pool->playerFreeList = malloc(sizeof(int) * playerCap);

  for (int i = 0; i < playerCap; i++) {
    pool->playerBullets[i] = NULL;
    pool->playerFreeList[i] = i + 1;
  }
  pool->playerFreeList[playerCap - 1] = -1; // End-of-list marker.
  pool->playerFreeListHead = 0;

  /* --- Initialize Enemy Bullet Sub-Pool --- */
  pool->enemyCapacity = enemyCap;
  pool->enemyActiveCount = 0;
  pool->enemyBullets = malloc(sizeof(Bullet *) * enemyCap);
  pool->enemyActiveIndices = malloc(sizeof(int) * enemyCap);
  pool->enemyFreeList = malloc(sizeof(int) * enemyCap);

  for (int i = 0; i < enemyCap; i++) {
    pool->enemyBullets[i] = NULL;
    pool->enemyFreeList[i] = i + 1;
  }
  pool->enemyFreeList[enemyCap - 1] = -1;
  pool->enemyFreeListHead = 0;
}

/*
 * Spawn a bullet into the appropriate sub-pool.
 */
void bulletPoolSpawn(BulletPool *pool, BulletSource source, float x, float y,
                     float vx, float vy) {
  if (source == BULLET_SOURCE_PLAYER) {
    // --- Player Bullet ---
    if (pool->playerFreeListHead == -1) {
      // No free slot for player bullet.
      return;
    }
    int index = pool->playerFreeListHead;
    pool->playerFreeListHead = pool->playerFreeList[index];

    if (pool->playerBullets[index] == NULL) {
      pool->playerBullets[index] = bulletCreate(source, x, y, vx, vy);
    } else {
      Bullet *bullet = pool->playerBullets[index];
      bullet->base.pos.x = x;
      bullet->base.pos.y = y;
      bullet->base.vel.x = vx;
      bullet->base.vel.y = vy;
      bullet->base.health = 1; // Reset "alive" status.
      bullet->source = source;
    }
    pool->playerActiveIndices[pool->playerActiveCount++] = index;
  } else if (source == BULLET_SOURCE_ENEMY) {
    // --- Enemy Bullet ---
    if (pool->enemyFreeListHead == -1) {
      // No free slot for enemy bullet.
      return;
    }
    int index = pool->enemyFreeListHead;
    pool->enemyFreeListHead = pool->enemyFreeList[index];

    if (pool->enemyBullets[index] == NULL) {
      pool->enemyBullets[index] = bulletCreate(source, x, y, vx, vy);
    } else {
      Bullet *bullet = pool->enemyBullets[index];
      bullet->base.pos.x = x;
      bullet->base.pos.y = y;
      bullet->base.vel.x = vx;
      bullet->base.vel.y = vy;
      bullet->base.health = 1; // Reset "alive" status.
      bullet->source = source;
    }
    pool->enemyActiveIndices[pool->enemyActiveCount++] = index;
  }
}

/*
 * Update all active bullets in both sub-pools.
 * If a bullet is dead (health <= 0), it is removed and its slot is freed.
 */
void bulletPoolUpdate(BulletPool *pool, float dt) {
  // --- Update Player Bullets ---
  for (int i = 0; i < pool->playerActiveCount;) {
    int index = pool->playerActiveIndices[i];
    Bullet *bullet = pool->playerBullets[index];
    if (bullet && bullet->base.health > 0) {
      bullet->base.update((Entity *)bullet, dt);
      i++; // Move to the next bullet.
    } else {
      // Bullet is dead. Remove it by swapping with the last active index.
      pool->playerActiveCount--;
      pool->playerActiveIndices[i] =
          pool->playerActiveIndices[pool->playerActiveCount];
      // Return the slot to the free list.
      pool->playerFreeList[index] = pool->playerFreeListHead;
      pool->playerFreeListHead = index;
    }
  }

  // --- Update Enemy Bullets ---
  for (int i = 0; i < pool->enemyActiveCount;) {
    int index = pool->enemyActiveIndices[i];
    Bullet *bullet = pool->enemyBullets[index];
    if (bullet && bullet->base.health > 0) {
      bullet->base.update((Entity *)bullet, dt);
      i++; // Move to next.
    } else {
      pool->enemyActiveCount--;
      pool->enemyActiveIndices[i] =
          pool->enemyActiveIndices[pool->enemyActiveCount];
      pool->enemyFreeList[index] = pool->enemyFreeListHead;
      pool->enemyFreeListHead = index;
    }
  }
}

/*
 * Clean up all resources allocated for the bullet pool.
 */
void bulletPoolDestroy(BulletPool *pool) {
  // --- Destroy Player Bullets ---
  for (int i = 0; i < pool->playerCapacity; i++) {
    if (pool->playerBullets[i]) {
      free(pool->playerBullets[i]);
      pool->playerBullets[i] = NULL;
    }
  }
  free(pool->playerBullets);
  free(pool->playerActiveIndices);
  free(pool->playerFreeList);

  // --- Destroy Enemy Bullets ---
  for (int i = 0; i < pool->enemyCapacity; i++) {
    if (pool->enemyBullets[i]) {
      free(pool->enemyBullets[i]);
      pool->enemyBullets[i] = NULL;
    }
  }
  free(pool->enemyBullets);
  free(pool->enemyActiveIndices);
  free(pool->enemyFreeList);
}

