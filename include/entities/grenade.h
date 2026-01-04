#ifndef GRENADE_H
#define GRENADE_H

/**
 * @file grenade.h
 * @brief Grenade entity for explosive combat.
 *
 * Provides functionality for throwable grenades that can be used
 * by both players and enemies to create explosive area damage.
 */

#include "entities/entity.h"
#include <stdbool.h>

/**
 * @brief Source of the grenade (who threw it).
 */
typedef enum {
  GRENADE_SOURCE_PLAYER, /**< Thrown by player */
  GRENADE_SOURCE_ENEMY   /**< Thrown by enemy */
} GrenadeSource;

/**
 * @brief State of the grenade.
 */
typedef enum {
  GRENADE_STATE_FLYING,    /**< In flight */
  GRENADE_STATE_BOUNCING,  /**< Bouncing on ground */
  GRENADE_STATE_EXPLODING, /**< Exploding */
  GRENADE_STATE_DONE       /**< Explosion complete */
} GrenadeState;

/**
 * @brief Grenade entity structure.
 */
typedef struct Grenade {
  Entity base;           /**< Base entity data */
  GrenadeSource source;  /**< Who threw the grenade */
  GrenadeState state;    /**< Current state */
  float fuseTimer;       /**< Time until explosion */
  float explosionRadius; /**< Radius of explosion damage */
  int explosionDamage;   /**< Damage dealt by explosion */
  float explosionTimer;  /**< Timer for explosion animation */
  float rotation;        /**< Rotation angle for visual effect */
} Grenade;

/* Creation / Destruction */

/**
 * @brief Create a new grenade.
 * @param source Who threw the grenade.
 * @param x Initial X position.
 * @param y Initial Y position.
 * @param vx Initial X velocity.
 * @param vy Initial Y velocity.
 * @return Pointer to new grenade, or NULL on failure.
 */
Grenade *grenadeCreate(GrenadeSource source, float x, float y, float vx, float vy);

/**
 * @brief Destroy a grenade and free its resources.
 * @param grenade The grenade to destroy.
 */
void grenadeDestroy(Grenade *grenade);

/* Update */

/**
 * @brief Update grenade state.
 * @param grenade The grenade to update.
 * @param dt Delta time in seconds.
 */
void grenadeUpdate(Grenade *grenade, float dt);

/* Queries */

/**
 * @brief Check if grenade is still active (not done exploding).
 * @param grenade The grenade to check.
 * @return true if active, false if done.
 */
bool isGrenadeActive(const Grenade *grenade);

/**
 * @brief Check if grenade is currently exploding.
 * @param grenade The grenade to check.
 * @return true if exploding, false otherwise.
 */
bool isGrenadeExploding(const Grenade *grenade);

/**
 * @brief Get the explosion bounds for damage checking.
 * @param grenade The exploding grenade.
 * @return SDL_Rect representing the explosion area.
 */
SDL_Rect grenadeGetExplosionBounds(const Grenade *grenade);

#endif /* GRENADE_H */
