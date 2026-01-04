#ifndef GRENADE_POOL_H
#define GRENADE_POOL_H

/**
 * @file grenade_pool.h
 * @brief Pool for managing active grenades.
 *
 * Provides allocation and lifecycle management for grenades,
 * using a fixed-size array for efficient access.
 */

#include "entities/grenade.h"

/** Maximum number of simultaneous grenades */
#define MAX_GRENADES 20

/**
 * @brief Pool for managing grenades.
 */
typedef struct GrenadePool {
  Grenade *grenades[MAX_GRENADES]; /**< Array of grenade pointers */
  int activeCount;                 /**< Number of active grenades */
} GrenadePool;

/**
 * @brief Initialize a grenade pool.
 *
 * Sets all slots to NULL and resets the active count.
 *
 * @param pool The pool to initialize (must not be NULL).
 */
void grenadePoolInit(GrenadePool *pool);

/**
 * @brief Destroy a grenade pool and free all grenades.
 *
 * @param pool The pool to destroy (may be NULL).
 */
void grenadePoolDestroy(GrenadePool *pool);

/**
 * @brief Add a grenade to the pool.
 *
 * @param pool The pool to add to (must not be NULL).
 * @param grenade The grenade to add (must not be NULL).
 * @return 0 on success, -1 if pool is full or invalid parameters.
 */
int grenadePoolAdd(GrenadePool *pool, Grenade *grenade);

/**
 * @brief Update all grenades in the pool.
 *
 * Updates each active grenade and removes inactive ones.
 *
 * @param pool The pool to update (must not be NULL).
 * @param dt Delta time in seconds.
 */
void grenadePoolUpdate(GrenadePool *pool, float dt);

/**
 * @brief Remove inactive grenades from the pool.
 *
 * Frees and removes grenades that are no longer active.
 *
 * @param pool The pool to clean up (must not be NULL).
 */
void grenadePoolCleanup(GrenadePool *pool);

/**
 * @brief Throw a grenade from a position.
 *
 * Creates a new grenade with calculated trajectory.
 *
 * @param pool The pool to add to (must not be NULL).
 * @param source Who threw the grenade.
 * @param x Starting X position.
 * @param y Starting Y position.
 * @param targetX Target X position (for calculating arc).
 * @param targetY Target Y position.
 */
void grenadePoolThrow(GrenadePool *pool, GrenadeSource source, float x, float y,
                      float targetX, float targetY);

#endif /* GRENADE_POOL_H */
