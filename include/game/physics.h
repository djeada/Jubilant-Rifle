#ifndef PHYSICS_H
#define PHYSICS_H

/**
 * @file physics.h
 * @brief Physics and collision detection system.
 *
 * Provides collision detection between game entities (bullets, player, enemies)
 * and physics simulation including gravity, platform collision resolution.
 */

#include "SDL2/SDL.h"
#include "entities/bullet_pool.h"
#include "entities/enemy_array.h"
#include "entities/player.h"
#include "map/map.h"
#include "map/trap.h"

/**
 * @brief Handle all collision detection between game entities.
 *
 * Processes collisions between bullets and entities, and handles
 * bullet culling based on distance from player.
 *
 * @param pool Bullet pool containing all active bullets (must not be NULL).
 * @param player The player entity (must not be NULL).
 * @param enemies Array of enemies (must not be NULL).
 * @param traps Optional trap array (may be NULL).
 */
void handleCollisions(BulletPool *restrict pool, Player *restrict player,
                      EnemyArray *restrict enemies, TrapArray *traps);

/**
 * @brief Apply physics simulation to all entities.
 *
 * Updates entity positions based on velocity, applies gravity,
 * and resolves platform collisions.
 *
 * @param player The player entity (must not be NULL).
 * @param enemies Array of enemies (must not be NULL).
 * @param map The game map containing platforms (must not be NULL).
 * @param dt Delta time in seconds.
 */
void applyPhysics(Player *restrict player, EnemyArray *restrict enemies,
                  const Map *restrict map, float dt);

#endif /* PHYSICS_H */
