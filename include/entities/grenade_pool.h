#ifndef GRENADE_POOL_H
#define GRENADE_POOL_H

/**
 * @file grenade_pool.h
 * @brief Pool for managing active grenades.
 */

#include "entities/grenade.h"

#define MAX_GRENADES 20

/**
 * @brief Pool for managing grenades.
 */
typedef struct GrenadePool {
  Grenade *grenades[MAX_GRENADES]; /**< Array of grenade pointers */
  int activeCount;                  /**< Number of active grenades */
} GrenadePool;

/**
 * @brief Initialize a grenade pool.
 * @param pool The pool to initialize.
 */
void grenadePoolInit(GrenadePool *pool);

/**
 * @brief Destroy a grenade pool and free all grenades.
 * @param pool The pool to destroy.
 */
void grenadePoolDestroy(GrenadePool *pool);

/**
 * @brief Add a grenade to the pool.
 * @param pool The pool to add to.
 * @param grenade The grenade to add.
 * @return 0 on success, -1 if pool is full.
 */
int grenadePoolAdd(GrenadePool *pool, Grenade *grenade);

/**
 * @brief Update all grenades in the pool.
 * @param pool The pool to update.
 * @param dt Delta time in seconds.
 */
void grenadePoolUpdate(GrenadePool *pool, float dt);

/**
 * @brief Remove inactive grenades from the pool.
 * @param pool The pool to clean up.
 */
void grenadePoolCleanup(GrenadePool *pool);

/**
 * @brief Throw a grenade from a position.
 * @param pool The pool to add to.
 * @param source Who threw the grenade.
 * @param x Starting X position.
 * @param y Starting Y position.
 * @param targetX Target X position (for calculating arc).
 * @param targetY Target Y position.
 */
void grenadePoolThrow(GrenadePool *pool, GrenadeSource source,
                       float x, float y, float targetX, float targetY);

#endif /* GRENADE_POOL_H */
