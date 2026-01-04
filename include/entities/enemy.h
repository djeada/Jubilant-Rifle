#ifndef ENEMY_H
#define ENEMY_H

#include "entities/bullet_pool.h"
#include "entities/entity.h"

/**
 * @brief State of the enemy.
 */
typedef enum {
  ENEMY_STATE_NORMAL,      /**< Normal ground-based enemy */
  ENEMY_STATE_PARACHUTING, /**< Descending on parachute */
  ENEMY_STATE_LANDING      /**< Just landed from parachute */
} EnemyState;

typedef struct {
  Entity base;
  float shootTimer;
  EnemyState state;        /**< Current enemy state */
  float parachuteSpeed;    /**< Descent speed while parachuting */
  float stateTimer;        /**< Timer for state transitions */
  int grenadeCount;        /**< Number of grenades available */
  float grenadeTimer;      /**< Cooldown timer for grenade throws */
} Enemy;

/* Creation / Destruction */
Enemy *enemyCreate(float x, float y);
void enemyDestroy(Enemy *enemy);

/* Update */
void enemyUpdate(Entity *self, float dt);

/* Actions */
void enemySetHorizontalVelocity(Enemy *enemy, float vx);
void enemySetVerticalVelocity(Enemy *enemy, float vy);
void enemyStopHorizontal(Enemy *enemy);
void enemyStopVertical(Enemy *enemy);
void enemyShoot(Enemy *enemy, BulletPool *bulletPool);

/* Queries */
bool isEnemyAlive(const Enemy *enemy);
Direction getEnemyFacingDirection(const Enemy *enemy);
bool isEnemyMoving(const Enemy *enemy);

/**
 * @brief Check if enemy is parachuting.
 * @param enemy The enemy to check.
 * @return true if parachuting, false otherwise.
 */
bool isEnemyParachuting(const Enemy *enemy);

/**
 * @brief Create a parachuting enemy.
 * @param x Spawn X position.
 * @param y Spawn Y position (typically above the screen).
 * @param targetY Target Y position where enemy will land.
 * @return Pointer to new parachuting enemy, or NULL on failure.
 */
Enemy *enemyCreateParachuting(float x, float y, float targetY);

/**
 * @brief Add grenades to enemy inventory.
 * @param enemy The enemy.
 * @param count Number of grenades to add.
 */
void enemyAddGrenades(Enemy *enemy, int count);

/**
 * @brief Check if enemy can throw a grenade.
 * @param enemy The enemy.
 * @return true if grenade is available and cooldown is ready.
 */
bool enemyCanThrowGrenade(const Enemy *enemy);

/**
 * @brief Use a grenade (decrement count and reset timer).
 * @param enemy The enemy.
 * @return true if grenade was used, false otherwise.
 */
bool enemyUseGrenade(Enemy *enemy);

#endif // ENEMY_H

