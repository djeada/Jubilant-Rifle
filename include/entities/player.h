#ifndef PLAYER_H
#define PLAYER_H

#include "entities/bullet_pool.h"
#include "entities/entity.h"

typedef struct Player {
  Entity base;
  int level;
  bool isClimbing;        /**< Whether player is on a ladder */
  bool climbIntentUp;     /**< Whether player is trying to climb up */
  bool climbIntentDown;   /**< Whether player is trying to climb down */
  bool jumpRequested;     /**< Whether a jump was requested this frame */
  bool isMeleeAttacking;  /**< Whether player is performing melee attack */
  float meleeTimer;       /**< Cooldown timer for melee attacks */
  float meleeDamage;      /**< Damage dealt by melee attacks */
  int grenadeCount;       /**< Number of grenades available */
  int jumpsUsed;          /**< Number of jumps used since last landing */
} Player;

/* Creation / Destruction */
Player *playerCreate(float x, float y);
void playerDestroy(Player *player);

/* Update */
void playerUpdate(Entity *entity, float dt);

/* Actions */
void playerSetHorizontalVelocity(Player *player, float vx);
void playerSetVerticalVelocity(Player *player, float vy);
void playerStopHorizontal(Player *player);
void playerStopVertical(Player *player);
void playerShoot(Player *player, BulletPool *bulletPool);

/**
 * @brief Perform a melee attack.
 * @param player The player performing the attack.
 */
void playerMeleeAttack(Player *player);

/**
 * @brief Check if player melee attack can hit a target.
 * @param player The attacking player.
 * @param targetRect The target's bounding rect.
 * @return true if melee attack hits, false otherwise.
 */
bool playerMeleeHits(const Player *player, const SDL_Rect *targetRect);

/**
 * @brief Get melee attack damage.
 * @param player The player.
 * @return Damage dealt by melee attack.
 */
int playerGetMeleeDamage(const Player *player);

/**
 * @brief Start climbing a ladder.
 * @param player The player.
 */
void playerStartClimbing(Player *player);

/**
 * @brief Stop climbing a ladder.
 * @param player The player.
 */
void playerStopClimbing(Player *player);

/**
 * @brief Check if player is currently climbing.
 * @param player The player.
 * @return true if climbing, false otherwise.
 */
bool playerIsClimbing(const Player *player);

/**
 * @brief Add grenades to player inventory.
 * @param player The player.
 * @param count Number of grenades to add.
 */
void playerAddGrenades(Player *player, int count);

/**
 * @brief Use a grenade (decrement count).
 * @param player The player.
 * @return true if grenade was available and used, false otherwise.
 */
bool playerUseGrenade(Player *player);

/**
 * @brief Get current grenade count.
 * @param player The player.
 * @return Number of grenades available.
 */
int playerGetGrenadeCount(const Player *player);

/* Queries */
bool isPlayerAlive(const Player *player);
Direction getPlayerFacingDirection(const Player *player);

#endif // PLAYER_H
