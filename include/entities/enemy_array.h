#ifndef ENEMY_ARRAY_H
#define ENEMY_ARRAY_H

/**
 * @file enemy_array.h
 * @brief Dynamic array for enemy management.
 *
 * Provides a resizable array for storing and updating enemies.
 * Handles automatic growth and removal of dead enemies.
 */

#include "entities/bullet_pool.h"
#include "entities/enemy.h"
#include "entities/entity.h"
#include "entities/player.h"
#include <stddef.h>
#include <stdlib.h>

/**
 * @brief Dynamic array of enemies.
 */
typedef struct EnemyArray {
  Enemy **data;    /**< Array of enemy pointers */
  int count;       /**< Current number of enemies */
  int capacity;    /**< Allocated capacity */
} EnemyArray;

/**
 * @brief Initialize an enemy array.
 *
 * Allocates initial capacity for the array.
 *
 * @param arr The array to initialize (must not be NULL).
 */
void enemyArrayInit(EnemyArray *arr);

/**
 * @brief Add an enemy to the array.
 *
 * Automatically grows the array if needed.
 *
 * @param arr The array to add to (must not be NULL).
 * @param enemy The enemy to add (must not be NULL).
 */
void enemyArrayAdd(EnemyArray *arr, Enemy *enemy);

/**
 * @brief Update all enemies and remove dead ones.
 *
 * Updates each alive enemy and handles shooting logic.
 * Dead enemies are removed by swapping with the last element.
 *
 * @param arr The array to update (must not be NULL).
 * @param dt Delta time in seconds.
 * @param pool Bullet pool for enemy shooting (must not be NULL).
 * @param player Player reference for AI targeting (may be NULL).
 */
void enemyArrayUpdate(EnemyArray *arr, float dt, BulletPool *pool,
                      const Player *player);

/**
 * @brief Destroy the array and free all enemies.
 *
 * @param arr The array to destroy (may be NULL).
 */
void enemyArrayDestroy(EnemyArray *arr);

#endif /* ENEMY_ARRAY_H */
