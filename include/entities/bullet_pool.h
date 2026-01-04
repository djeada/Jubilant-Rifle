#ifndef BULLET_POOL_H
#define BULLET_POOL_H

/**
 * @file bullet_pool.h
 * @brief Object pool for efficient bullet management.
 *
 * Provides a pool-based allocation system for bullets to avoid
 * frequent malloc/free calls during gameplay. Maintains separate
 * sub-pools for player and enemy bullets for efficient collision
 * detection.
 */

#include "entities/bullet.h"
#include <SDL2/SDL.h>
#include <stddef.h>
#include <stdlib.h>

/**
 * @brief Pool for managing bullet instances.
 *
 * Uses separate sub-pools for player and enemy bullets with
 * free list-based allocation for O(1) spawn/despawn operations.
 */
typedef struct BulletPool {
  /* Player bullet sub-pool */
  Bullet **playerBullets;      /**< Array of pointers to player bullets */
  int playerCapacity;          /**< Capacity for player bullets */
  int *playerActiveIndices;    /**< Indices of active player bullets */
  int playerActiveCount;       /**< Number of active player bullets */
  int *playerFreeList;         /**< Free list for available slots */
  int playerFreeListHead;      /**< Head of player free list (-1 if empty) */

  /* Enemy bullet sub-pool */
  Bullet **enemyBullets;       /**< Array of pointers to enemy bullets */
  int enemyCapacity;           /**< Capacity for enemy bullets */
  int *enemyActiveIndices;     /**< Indices of active enemy bullets */
  int enemyActiveCount;        /**< Number of active enemy bullets */
  int *enemyFreeList;          /**< Free list for available slots */
  int enemyFreeListHead;       /**< Head of enemy free list (-1 if empty) */
} BulletPool;

/**
 * @brief Initialize a bullet pool.
 *
 * Allocates and initializes pool structures. The total capacity
 * is split evenly between player and enemy sub-pools.
 *
 * @param pool The pool to initialize (must not be NULL).
 * @param totalCapacity Total number of bullets to allocate.
 */
void bulletPoolInit(BulletPool *pool, int totalCapacity);

/**
 * @brief Spawn a bullet into the pool.
 *
 * Creates or reuses a bullet at the specified position with
 * the given velocity.
 *
 * @param pool The pool to spawn into (must not be NULL).
 * @param source Whether this is a player or enemy bullet.
 * @param x Initial X position.
 * @param y Initial Y position.
 * @param vx Initial X velocity.
 * @param vy Initial Y velocity.
 */
void bulletPoolSpawn(BulletPool *pool, BulletSource source, float x, float y,
                     float vx, float vy);

/**
 * @brief Update all active bullets.
 *
 * Calls update on each active bullet and removes dead bullets
 * from the active list.
 *
 * @param pool The pool to update (must not be NULL).
 * @param dt Delta time in seconds.
 */
void bulletPoolUpdate(BulletPool *pool, float dt);

/**
 * @brief Destroy the bullet pool and free all resources.
 *
 * @param pool The pool to destroy (must not be NULL).
 */
void bulletPoolDestroy(BulletPool *pool);

#endif /* BULLET_POOL_H */

