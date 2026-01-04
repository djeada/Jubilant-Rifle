#ifndef TRAP_H
#define TRAP_H

/**
 * @file trap.h
 * @brief Environmental traps for strategic combat.
 *
 * Provides functionality for environmental hazards like fuel storage
 * that can cause additional damage when triggered.
 */

#include "utils/point.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief Type of environmental trap.
 */
typedef enum {
  TRAP_TYPE_FUEL_BARREL,    /**< Explodes when shot, causes fire */
  TRAP_TYPE_SPIKE_PIT,      /**< Damage on contact */
  TRAP_TYPE_ELECTRIC_FENCE, /**< Periodic shock damage */
  TRAP_TYPE_FIRE_GRATE      /**< Continuous fire damage */
} TrapType;

/**
 * @brief State of a trap.
 */
typedef enum {
  TRAP_STATE_INACTIVE,   /**< Not triggered */
  TRAP_STATE_ACTIVE,     /**< Currently damaging */
  TRAP_STATE_TRIGGERED,  /**< One-time trap activated */
  TRAP_STATE_DESTROYED   /**< Trap destroyed */
} TrapState;

/**
 * @brief Represents an environmental trap.
 */
typedef struct Trap {
  SDL_Rect rect;           /**< Trap bounds and trigger zone */
  TrapType type;           /**< Type of trap */
  TrapState state;         /**< Current state */
  int damage;              /**< Damage dealt per activation */
  float damageInterval;    /**< Time between damage ticks (for continuous) */
  float damageTimer;       /**< Timer for damage ticks */
  float effectRadius;      /**< Radius of effect (for explosions) */
  float effectDuration;    /**< Duration of effect (for fire, etc.) */
  float effectTimer;       /**< Timer for ongoing effects */
  int health;              /**< Health for destructible traps */
} Trap;

/**
 * @brief Array of traps in a map.
 */
typedef struct TrapArray {
  Trap *traps;       /**< Array of traps */
  size_t count;      /**< Number of traps */
  size_t capacity;   /**< Allocated capacity */
} TrapArray;

/**
 * @brief Initialize a trap array.
 * @param arr The array to initialize.
 */
void trapArrayInit(TrapArray *arr);

/**
 * @brief Destroy a trap array and free resources.
 * @param arr The array to destroy.
 */
void trapArrayDestroy(TrapArray *arr);

/**
 * @brief Add a trap to the array.
 * @param arr The array to add to.
 * @param trap The trap to add.
 * @return 0 on success, -1 on failure.
 */
int trapArrayAdd(TrapArray *arr, const Trap *trap);

/**
 * @brief Update all traps.
 * @param arr The trap array.
 * @param dt Delta time in seconds.
 */
void trapArrayUpdate(TrapArray *arr, float dt);

/**
 * @brief Check for trap damage to an entity.
 * @param arr The trap array.
 * @param entityRect The entity's bounding rect.
 * @return Total damage to apply, or 0 if none.
 */
int trapArrayCheckDamage(TrapArray *arr, const SDL_Rect *entityRect);

/**
 * @brief Trigger a trap at a specific position (e.g., from bullet hit).
 * @param arr The trap array.
 * @param x X coordinate of trigger point.
 * @param y Y coordinate of trigger point.
 * @return true if a trap was triggered, false otherwise.
 */
bool trapArrayTriggerAt(TrapArray *arr, float x, float y);

/**
 * @brief Damage a trap (for destructible traps).
 * @param arr The trap array.
 * @param trapIndex Index of trap to damage.
 * @param damage Amount of damage.
 * @return true if trap was destroyed, false otherwise.
 */
bool trapArrayDamageTrap(TrapArray *arr, size_t trapIndex, int damage);

/**
 * @brief Reset all traps to initial state.
 * @param arr The trap array.
 */
void trapArrayReset(TrapArray *arr);

/**
 * @brief Parse traps from JSON data.
 * @param json The JSON string to parse.
 * @param arr The array to populate.
 * @return 0 on success, -1 on failure.
 */
int parseTraps(const char *json, TrapArray *arr);

/**
 * @brief Count traps in JSON data.
 * @param json The JSON string to parse.
 * @return Number of traps found, or -1 on error.
 */
int countTrapsInJson(const char *json);

#endif /* TRAP_H */
