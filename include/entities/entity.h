#ifndef ENTITY_H
#define ENTITY_H

/**
 * @file entity.h
 * @brief Base entity system for game objects.
 *
 * Provides the foundation for all game entities including players,
 * enemies, and bullets. Uses a component-like pattern for shared
 * behavior and polymorphism through function pointers.
 */

#include "utils/point.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* ---------------------------------------------------------------------------
 * Animation System
 * --------------------------------------------------------------------------- */

/**
 * @brief Animation data for animated entities.
 */
typedef struct Animation {
  SDL_Texture *tex;     /**< Texture containing animation frames */
  int frameCount;       /**< Total number of frames in the animation */
  float frameDuration;  /**< Duration of each frame in seconds */
  float timer;          /**< Current animation timer */
  int currentFrame;     /**< Currently displayed frame index */
} Animation;

/* ---------------------------------------------------------------------------
 * Entity Types and Direction
 * --------------------------------------------------------------------------- */

/**
 * @brief Types of entities in the game.
 */
typedef enum EntityType {
  ENTITY_PLAYER, /**< Player-controlled character */
  ENTITY_ENEMY,  /**< AI-controlled enemy */
  ENTITY_BULLET  /**< Projectile entity */
} EntityType;

/**
 * @brief Facing direction for entities.
 */
typedef enum Direction {
  DIRECTION_LEFT,  /**< Facing left */
  DIRECTION_RIGHT  /**< Facing right */
} Direction;

/* ---------------------------------------------------------------------------
 * Base Entity Structure
 * --------------------------------------------------------------------------- */

/**
 * @brief Base entity structure for all game objects.
 *
 * This structure provides common properties and behaviors for all
 * entities in the game. Derived types embed this structure and
 * extend it with their own data.
 */
typedef struct Entity {
  EntityType type;     /**< Type of entity */
  Point pos;           /**< Position in world coordinates */
  Point vel;           /**< Velocity (units per second) */
  int health;          /**< Current health points */
  Direction direction; /**< Facing direction */
  Animation *anim;     /**< Animation data (can be NULL) */

  /**
   * @brief Update function pointer for polymorphic behavior.
   * @param self Pointer to the entity.
   * @param dt Delta time in seconds.
   */
  void (*update)(struct Entity *self, float dt);
} Entity;

/* ---------------------------------------------------------------------------
 * Entity API
 * --------------------------------------------------------------------------- */

/**
 * @brief Create a new entity.
 *
 * Allocates and initializes a new entity with default values.
 *
 * @param type The type of entity to create.
 * @param x Initial X position.
 * @param y Initial Y position.
 * @return Pointer to the new entity, or NULL on failure.
 */
Entity *entityCreate(EntityType type, float x, float y);

/**
 * @brief Destroy an entity and free its resources.
 *
 * Frees the entity and any associated animation data.
 *
 * @param e The entity to destroy (may be NULL).
 */
void entityDestroy(Entity *e);

/**
 * @brief Common update logic shared by all entities.
 *
 * Updates position based on velocity and advances animation.
 *
 * @param self The entity to update (must not be NULL).
 * @param dt Delta time in seconds (must be >= 0).
 */
void commonEntityUpdate(Entity *self, float dt);

/**
 * @brief Check if an entity is alive (health > 0).
 *
 * @param e The entity to check (may be NULL).
 * @return true if entity exists and is alive, false otherwise.
 */
bool isEntityAlive(const Entity *e);

/**
 * @brief Check if an entity is moving.
 *
 * @param e The entity to check (may be NULL).
 * @return true if velocity is non-zero, false otherwise.
 */
bool isEntityMoving(const Entity *e);

/**
 * @brief Get the bounding rectangle of an entity.
 *
 * @param e The entity (may be NULL).
 * @param width Width of the entity.
 * @param height Height of the entity.
 * @return SDL_Rect representing the entity bounds, or zero rect if NULL.
 */
SDL_Rect entityGetBounds(const Entity *e, int width, int height);

#endif /* ENTITY_H */

