#include "entities/bullet_pool.h"
#include "entities/bullet.h" // For bulletUpdate and other bullet-specific logic

// Initialize the bullet pool with a specified capacity.
void bulletPoolInit(BulletPool *pool, int capacity) {
  pool->capacity = capacity;
  pool->activeCount = 0;
  pool->bullets = malloc(sizeof(Entity *) * capacity);
  pool->activeIndices = malloc(sizeof(int) * capacity);
  pool->freeList = malloc(sizeof(int) * capacity);

  // Initialize all bullet slots to NULL and build the free list.
  for (int i = 0; i < capacity; i++) {
    pool->bullets[i] = NULL;
    pool->freeList[i] = i + 1;
  }
  pool->freeList[capacity - 1] = -1; // End-of-list marker.
  pool->freeListHead = 0;
}

// Spawn a bullet with the given position and velocity.
void bulletPoolSpawn(BulletPool *pool, float x, float y, float vx, float vy) {
  // Check if there is an available slot.
  if (pool->freeListHead == -1) {
    // No available bullet slot; optionally expand the pool or ignore the spawn.
    return;
  }

  // Pop an index from the free list.
  int index = pool->freeListHead;
  pool->freeListHead = pool->freeList[index];

  // Create or recycle the bullet entity.
  if (pool->bullets[index] == NULL) {
    pool->bullets[index] = entityCreate(ENTITY_BULLET, x, y);
    pool->bullets[index]->update = bulletUpdate;
  } else {
    // Recycle an existing bullet.
    pool->bullets[index]->pos.x = x;
    pool->bullets[index]->pos.y = y;
    pool->bullets[index]->health = 1; // Reset its "alive" status.
  }
  pool->bullets[index]->vel.x = vx;
  pool->bullets[index]->vel.y = vy;

  // Add the index to the active list.
  pool->activeIndices[pool->activeCount++] = index;
}

// Update all active bullets. Dead bullets are removed from the active list
// and their indices are returned to the free list.
void bulletPoolUpdate(BulletPool *pool, float dt) {
  for (int i = 0; i < pool->activeCount;) {
    int index = pool->activeIndices[i];
    Entity *bullet = pool->bullets[index];

    if (bullet && bullet->health > 0) {
      if (bullet->update)
        bullet->update(bullet, dt);
      i++; // Proceed to next active bullet.
    } else {
      // Bullet is dead; remove it from active list by swapping with the last
      // active index.
      pool->activeCount--;
      pool->activeIndices[i] = pool->activeIndices[pool->activeCount];

      // Return the bullet slot to the free list.
      pool->freeList[index] = pool->freeListHead;
      pool->freeListHead = index;
    }
  }
}

// Destroy the bullet pool and free all allocated resources.
void bulletPoolDestroy(BulletPool *pool) {
  for (int i = 0; i < pool->capacity; i++) {
    if (pool->bullets[i]) {
      entityDestroy(pool->bullets[i]);
      pool->bullets[i] = NULL;
    }
  }
  free(pool->bullets);
  free(pool->activeIndices);
  free(pool->freeList);
}
