#include "entities/bullet_pool.h"
#include "utils/consts.h"
#include <stdlib.h>

/* ---------------------------------------------------------------------------
 * Pool Initialization
 * --------------------------------------------------------------------------- */

void bulletPoolInit(BulletPool *pool, int totalCapacity) {
  if (!pool || totalCapacity <= 0)
    return;

  /* Split capacity between player and enemy bullets */
  const int playerCap = totalCapacity / 2;
  const int enemyCap = totalCapacity - playerCap;

  /* Initialize player bullet sub-pool */
  pool->playerCapacity = playerCap;
  pool->playerActiveCount = 0;
  pool->playerBullets = malloc(sizeof(Bullet *) * (size_t)playerCap);
  pool->playerActiveIndices = malloc(sizeof(int) * (size_t)playerCap);
  pool->playerFreeList = malloc(sizeof(int) * (size_t)playerCap);

  if (!pool->playerBullets || !pool->playerActiveIndices ||
      !pool->playerFreeList) {
    /* Handle allocation failure */
    free(pool->playerBullets);
    free(pool->playerActiveIndices);
    free(pool->playerFreeList);
    pool->playerBullets = NULL;
    pool->playerActiveIndices = NULL;
    pool->playerFreeList = NULL;
    pool->playerCapacity = 0;
  } else {
    for (int i = 0; i < playerCap; i++) {
      pool->playerBullets[i] = NULL;
      pool->playerFreeList[i] = i + 1;
    }
    pool->playerFreeList[playerCap - 1] = -1; /* End-of-list marker */
    pool->playerFreeListHead = 0;
  }

  /* Initialize enemy bullet sub-pool */
  pool->enemyCapacity = enemyCap;
  pool->enemyActiveCount = 0;
  pool->enemyBullets = malloc(sizeof(Bullet *) * (size_t)enemyCap);
  pool->enemyActiveIndices = malloc(sizeof(int) * (size_t)enemyCap);
  pool->enemyFreeList = malloc(sizeof(int) * (size_t)enemyCap);

  if (!pool->enemyBullets || !pool->enemyActiveIndices ||
      !pool->enemyFreeList) {
    /* Handle allocation failure */
    free(pool->enemyBullets);
    free(pool->enemyActiveIndices);
    free(pool->enemyFreeList);
    pool->enemyBullets = NULL;
    pool->enemyActiveIndices = NULL;
    pool->enemyFreeList = NULL;
    pool->enemyCapacity = 0;
  } else {
    for (int i = 0; i < enemyCap; i++) {
      pool->enemyBullets[i] = NULL;
      pool->enemyFreeList[i] = i + 1;
    }
    pool->enemyFreeList[enemyCap - 1] = -1;
    pool->enemyFreeListHead = 0;
  }
}

/* ---------------------------------------------------------------------------
 * Bullet Spawning
 * --------------------------------------------------------------------------- */

void bulletPoolSpawn(BulletPool *pool, BulletSource source, float x, float y,
                     float vx, float vy) {
  if (!pool)
    return;

  if (source == BULLET_SOURCE_PLAYER) {
    /* Player bullet */
    if (pool->playerFreeListHead == -1 || !pool->playerBullets)
      return; /* No free slot */

    const int index = pool->playerFreeListHead;
    pool->playerFreeListHead = pool->playerFreeList[index];

    if (pool->playerBullets[index] == NULL) {
      pool->playerBullets[index] = bulletCreate(source, x, y, vx, vy);
    } else {
      /* Reuse existing bullet */
      Bullet *restrict bullet = pool->playerBullets[index];
      bullet->base.pos.x = x;
      bullet->base.pos.y = y;
      bullet->base.vel.x = vx;
      bullet->base.vel.y = vy;
      bullet->base.health = 1;
      bullet->source = source;
    }
    pool->playerActiveIndices[pool->playerActiveCount++] = index;

  } else if (source == BULLET_SOURCE_ENEMY) {
    /* Enemy bullet */
    if (pool->enemyFreeListHead == -1 || !pool->enemyBullets)
      return; /* No free slot */

    const int index = pool->enemyFreeListHead;
    pool->enemyFreeListHead = pool->enemyFreeList[index];

    if (pool->enemyBullets[index] == NULL) {
      pool->enemyBullets[index] = bulletCreate(source, x, y, vx, vy);
    } else {
      /* Reuse existing bullet */
      Bullet *restrict bullet = pool->enemyBullets[index];
      bullet->base.pos.x = x;
      bullet->base.pos.y = y;
      bullet->base.vel.x = vx;
      bullet->base.vel.y = vy;
      bullet->base.health = 1;
      bullet->source = source;
    }
    pool->enemyActiveIndices[pool->enemyActiveCount++] = index;
  }
}

/* ---------------------------------------------------------------------------
 * Pool Update
 * --------------------------------------------------------------------------- */

void bulletPoolUpdate(BulletPool *pool, float dt) {
  if (!pool)
    return;

  /* Update player bullets */
  if (pool->playerBullets && pool->playerActiveIndices && pool->playerFreeList) {
    for (int i = 0; i < pool->playerActiveCount;) {
      const int index = pool->playerActiveIndices[i];
      Bullet *restrict bullet = pool->playerBullets[index];

      if (bullet && bullet->base.health > 0) {
        if (bullet->base.update) {
          bullet->base.update((Entity *)bullet, dt);
        }
        i++;
      } else {
        /* Remove dead bullet by swapping with last active */
        pool->playerActiveCount--;
        pool->playerActiveIndices[i] =
            pool->playerActiveIndices[pool->playerActiveCount];

        /* Return slot to free list */
        pool->playerFreeList[index] = pool->playerFreeListHead;
        pool->playerFreeListHead = index;
      }
    }
  }

  /* Update enemy bullets */
  if (pool->enemyBullets && pool->enemyActiveIndices && pool->enemyFreeList) {
    for (int i = 0; i < pool->enemyActiveCount;) {
      const int index = pool->enemyActiveIndices[i];
      Bullet *restrict bullet = pool->enemyBullets[index];

      if (bullet && bullet->base.health > 0) {
        if (bullet->base.update) {
          bullet->base.update((Entity *)bullet, dt);
        }
        i++;
      } else {
        /* Remove dead bullet by swapping with last active */
        pool->enemyActiveCount--;
        pool->enemyActiveIndices[i] =
            pool->enemyActiveIndices[pool->enemyActiveCount];

        /* Return slot to free list */
        pool->enemyFreeList[index] = pool->enemyFreeListHead;
        pool->enemyFreeListHead = index;
      }
    }
  }
}

/* ---------------------------------------------------------------------------
 * Pool Destruction
 * --------------------------------------------------------------------------- */

void bulletPoolDestroy(BulletPool *pool) {
  if (!pool)
    return;

  /* Destroy player bullets */
  if (pool->playerBullets) {
    for (int i = 0; i < pool->playerCapacity; i++) {
      if (pool->playerBullets[i]) {
        free(pool->playerBullets[i]);
        pool->playerBullets[i] = NULL;
      }
    }
    free(pool->playerBullets);
    pool->playerBullets = NULL;
  }
  free(pool->playerActiveIndices);
  free(pool->playerFreeList);
  pool->playerActiveIndices = NULL;
  pool->playerFreeList = NULL;
  pool->playerCapacity = 0;
  pool->playerActiveCount = 0;

  /* Destroy enemy bullets */
  if (pool->enemyBullets) {
    for (int i = 0; i < pool->enemyCapacity; i++) {
      if (pool->enemyBullets[i]) {
        free(pool->enemyBullets[i]);
        pool->enemyBullets[i] = NULL;
      }
    }
    free(pool->enemyBullets);
    pool->enemyBullets = NULL;
  }
  free(pool->enemyActiveIndices);
  free(pool->enemyFreeList);
  pool->enemyActiveIndices = NULL;
  pool->enemyFreeList = NULL;
  pool->enemyCapacity = 0;
  pool->enemyActiveCount = 0;
}

