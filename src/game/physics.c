#include "game/physics.h"
#include "entities/enemy.h"
#include "map/ladder.h"
#include "utils/consts.h"
#include <limits.h>
#include <stddef.h>

/* ---------------------------------------------------------------------------
 * Physics Constants
 * --------------------------------------------------------------------------- */

/** Distance threshold for horizontal overlap detection */
#define PLATFORM_COLLISION_MARGIN 0

/** Chase activation thresholds for enemy AI */
#define ENEMY_CHASE_DISTANCE_X 400.0f
#define ENEMY_CHASE_DISTANCE_Y 120.0f

/** Shooting range thresholds for enemy AI */
#define ENEMY_SHOOT_DISTANCE_X 600.0f
#define ENEMY_SHOOT_DISTANCE_Y 200.0f

/* ---------------------------------------------------------------------------
 * Inline Helper Functions
 * --------------------------------------------------------------------------- */

/**
 * @brief Check if two horizontal ranges overlap.
 * @param x1 Start of first range.
 * @param w1 Width of first range.
 * @param x2 Start of second range.
 * @param w2 Width of second range.
 * @return true if ranges overlap, false otherwise.
 */
static inline bool rangesOverlap(int x1, int w1, int x2, int w2) {
  return (x1 + w1 > x2) && (x1 < x2 + w2);
}

/**
 * @brief Compute absolute value of a float.
 * @param x Input value.
 * @return Absolute value of x.
 */
static inline float absFloat(float x) {
  return (x < 0.0f) ? -x : x;
}

/**
 * @brief Clamp a float value to a range.
 * @param val Value to clamp.
 * @param min Minimum value.
 * @param max Maximum value.
 * @return Clamped value.
 */
static inline float clampFloat(float val, float min, float max) {
  if (val < min) return min;
  if (val > max) return max;
  return val;
}

/* ---------------------------------------------------------------------------
 * Platform Collision Detection
 * --------------------------------------------------------------------------- */

static const Platform *findSupportingPlatform(const Entity *restrict entity,
                                              const Map *restrict map,
                                              int width, int height) {
  if (!entity || !map || !map->platforms || map->platformCount == 0)
    return NULL;

  const int entityX = (int)entity->pos.x;
  const int entityBottom = (int)entity->pos.y + height;
  const Platform *best = NULL;
  int bestTop = INT_MAX;

  for (size_t i = 0; i < map->platformCount; i++) {
    const Platform *restrict platform = &map->platforms[i];

    /* Early exit: skip if no horizontal overlap */
    if (!rangesOverlap(entityX, width, platform->x, platform->w))
      continue;

    /* Check vertical collision with platform top surface */
    const int platformBottom = platform->y + platform->h;
    if (entityBottom >= platform->y && entityBottom <= platformBottom) {
      if (platform->y < bestTop) {
        bestTop = platform->y;
        best = platform;
      }
    }
  }

  return best;
}

static const Platform *resolvePlatformCollision(Entity *restrict entity,
                                                const Map *restrict map,
                                                int width, int height) {
  if (!entity || !map)
    return NULL;

  const Platform *platform = NULL;

  /* Only find supporting platform when falling (positive Y velocity) */
  if (entity->vel.y >= 0.0f) {
    platform = findSupportingPlatform(entity, map, width, height);
  }

  if (platform) {
    entity->pos.y = (float)(platform->y - height);
    entity->vel.y = 0.0f;
  } else if (map->rect.h > 0) {
    const float maxY = (float)(map->rect.h - height);
    if (entity->pos.y > maxY) {
      entity->pos.y = maxY;
      entity->vel.y = 0.0f;
    }
  }

  return platform;
}

/* ---------------------------------------------------------------------------
 * Player Physics
 * --------------------------------------------------------------------------- */

static void applyPlayerPhysics(Player *restrict player,
                               const Map *restrict map, float dt) {
  if (!player || !map)
    return;

  const Ladder *ladder = NULL;

  /* Check for ladder overlap */
  if (map->ladders) {
    const LadderArray *ladders = (const LadderArray *)map->ladders;
    SDL_Rect probeRect = {0, 0, 0, 0};

    if (player->climbIntentUp || player->climbIntentDown) {
      probeRect.x = (int)player->base.pos.x;
      probeRect.y = (int)(player->base.pos.y - 10);
      probeRect.w = SPRITE_WIDTH;
      probeRect.h = HUMANOID_FRAME_HEIGHT + 30;
    } else {
      probeRect.x = (int)(player->base.pos.x + (SPRITE_WIDTH / 2) - 5);
      probeRect.y = (int)(player->base.pos.y - 5);
      probeRect.w = 10;
      probeRect.h = HUMANOID_FRAME_HEIGHT + 10;
    }
    ladder = ladderArrayCheckOverlap(ladders, &probeRect);
  }

  /* Handle climbing state */
  if (ladder && (player->climbIntentUp || player->climbIntentDown)) {
    const bool wasClimbing = player->isClimbing;
    player->isClimbing = true;
    player->base.vel.x = 0.0f;

    if (!wasClimbing) {
      player->base.pos.x =
          (float)(ladder->rect.x + (ladder->rect.w - SPRITE_WIDTH) / 2);
    }

    player->base.vel.y = player->climbIntentUp ? -LADDER_CLIMB_SPEED
                                               : LADDER_CLIMB_SPEED;
  } else if (!ladder) {
    player->isClimbing = false;
  }

  /* Apply gravity or climbing physics */
  if (player->isClimbing) {
    if (!player->climbIntentUp && !player->climbIntentDown) {
      player->base.vel.y = 0.0f;
    }
  } else {
    /* Double jump logic */
    if (player->jumpRequested && player->jumpsUsed < 2) {
      player->base.vel.y = -PLAYER_JUMP_SPEED;
      player->jumpsUsed++;
    }
    player->base.vel.y += PLAYER_FALL_INCREMENT * dt;
  }
  player->jumpRequested = false;

  /* Update position and resolve collisions */
  commonEntityUpdate(&player->base, dt);
  resolvePlatformCollision(&player->base, map, SPRITE_WIDTH,
                           HUMANOID_FRAME_HEIGHT);

  /* Reset jump count when grounded */
  if (!player->isClimbing && player->base.vel.y == 0.0f) {
    player->jumpsUsed = 0;
  }
}

/* ---------------------------------------------------------------------------
 * Enemy Physics
 * --------------------------------------------------------------------------- */

static void applyEnemyPhysics(const Player *restrict player,
                              EnemyArray *restrict enemies,
                              const Map *restrict map, float dt) {
  if (!enemies || !map)
    return;

  const float chaseDistX = ENEMY_CHASE_DISTANCE_X;
  const float chaseDistY = ENEMY_CHASE_DISTANCE_Y;

  for (int i = 0; i < enemies->count; i++) {
    Enemy *restrict enemy = enemies->data[i];
    if (!enemy || !isEnemyAlive(enemy))
      continue;

    /* Handle parachuting enemies separately */
    if (enemy->state == ENEMY_STATE_PARACHUTING) {
      commonEntityUpdate(&enemy->base, dt);
      continue;
    }

    /* Apply gravity when not landing */
    if (enemy->state != ENEMY_STATE_LANDING) {
      enemy->base.vel.y += ENEMY_FALL_INCREMENT * dt;
    } else {
      enemy->base.vel.x = 0.0f;
    }

    commonEntityUpdate(&enemy->base, dt);

    const Platform *platform = resolvePlatformCollision(
        &enemy->base, map, SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT);

    /* AI behavior when on platform */
    if (enemy->state == ENEMY_STATE_NORMAL && platform) {
      const float speed = ENEMY_MOVE_SPEED;
      bool chasing = false;

      /* Player chase logic */
      if (player) {
        const float dx = absFloat(player->base.pos.x - enemy->base.pos.x);
        const float dy = absFloat(player->base.pos.y - enemy->base.pos.y);

        chasing = (dx <= chaseDistX && dy <= chaseDistY);
        if (chasing) {
          const float chaseDir =
              (player->base.pos.x > enemy->base.pos.x) ? speed : -speed;
          enemySetHorizontalVelocity(enemy, chaseDir);
        }
      }

      /* Patrol behavior when not chasing */
      if (!chasing && enemy->base.vel.x == 0.0f) {
        const float patrolDir =
            (enemy->base.direction == DIRECTION_RIGHT) ? speed : -speed;
        enemySetHorizontalVelocity(enemy, patrolDir);
      }

      /* Platform edge collision */
      const float leftEdge = (float)platform->x;
      const float rightEdge = (float)(platform->x + platform->w - SPRITE_WIDTH);

      if (enemy->base.pos.x <= leftEdge) {
        enemy->base.pos.x = leftEdge;
        enemySetHorizontalVelocity(enemy, speed);
      } else if (enemy->base.pos.x >= rightEdge) {
        enemy->base.pos.x = rightEdge;
        enemySetHorizontalVelocity(enemy, -speed);
      }
    }
  }
}

/* ---------------------------------------------------------------------------
 * Bullet Collision Detection
 * --------------------------------------------------------------------------- */

/**
 * @brief Cull bullets that have left the visible game area.
 *
 * Marks bullets as inactive when they move too far from the player,
 * preventing them from consuming resources indefinitely.
 */
static void handleBulletWindowCollision(BulletPool *restrict pool,
                                        const Player *restrict player) {
  if (!pool || !player)
    return;

  /* Define culling boundaries relative to player position */
  const float leftBound = player->base.pos.x - GAME_WIDTH;
  const float rightBound = player->base.pos.x + GAME_WIDTH;
  const float topBound = player->base.pos.y - GAME_HEIGHT;
  const float bottomBound = player->base.pos.y + GAME_HEIGHT;

  /* Cull player bullets */
  for (int i = 0; i < pool->playerActiveCount; i++) {
    const int index = pool->playerActiveIndices[i];
    Bullet *restrict bullet = pool->playerBullets[index];

    if (!bullet || !isEntityAlive(&bullet->base))
      continue;

    const float bx = bullet->base.pos.x;
    const float by = bullet->base.pos.y;

    if (bx < leftBound || bx > rightBound || by < topBound ||
        by > bottomBound) {
      bullet->base.health = 0;
    }
  }

  /* Cull enemy bullets */
  for (int i = 0; i < pool->enemyActiveCount; i++) {
    const int index = pool->enemyActiveIndices[i];
    Bullet *restrict bullet = pool->enemyBullets[index];

    if (!bullet || !isEntityAlive(&bullet->base))
      continue;

    const float bx = bullet->base.pos.x;
    const float by = bullet->base.pos.y;

    if (bx < leftBound || bx > rightBound || by < topBound ||
        by > bottomBound) {
      bullet->base.health = 0;
    }
  }
}

/**
 * @brief Handle bullet-entity collisions.
 *
 * Processes collisions between bullets and entities (player/enemies),
 * applying damage and marking collided bullets as dead.
 */
static void handleBulletEntityCollision(BulletPool *restrict pool,
                                        Player *restrict player,
                                        EnemyArray *restrict enemies,
                                        TrapArray *traps) {
  if (!pool || !player || !enemies)
    return;

  SDL_Rect bulletRect, targetRect;

  /* Pre-compute player hitbox */
  const SDL_Rect playerHitbox = {(int)player->base.pos.x,
                                 (int)player->base.pos.y, SPRITE_WIDTH,
                                 HUMANOID_FRAME_HEIGHT};

  /* --- Process enemy bullets hitting the player --- */
  for (int i = 0; i < pool->enemyActiveCount; i++) {
    const int index = pool->enemyActiveIndices[i];
    Bullet *restrict bullet = pool->enemyBullets[index];

    if (!bullet || !isEntityAlive(&bullet->base))
      continue;

    bulletRect.x = (int)bullet->base.pos.x;
    bulletRect.y = (int)bullet->base.pos.y;
    bulletRect.w = BULLET_WIDTH;
    bulletRect.h = BULLET_HEIGHT;

    if (SDL_HasIntersection(&bulletRect, &playerHitbox)) {
      player->base.health -= 10;
      bullet->base.health = 0;
      continue; /* Bullet consumed, skip trap check */
    }

    /* Check trap collisions for surviving bullets */
    if (traps && bullet->base.health > 0) {
      if (trapArrayTriggerAt(traps, bullet->base.pos.x, bullet->base.pos.y)) {
        bullet->base.health = 0;
      }
    }
  }

  /* --- Process player bullets hitting enemies --- */
  for (int i = 0; i < pool->playerActiveCount; i++) {
    const int index = pool->playerActiveIndices[i];
    Bullet *restrict bullet = pool->playerBullets[index];

    if (!bullet || !isEntityAlive(&bullet->base))
      continue;

    bulletRect.x = (int)bullet->base.pos.x;
    bulletRect.y = (int)bullet->base.pos.y;
    bulletRect.w = BULLET_WIDTH;
    bulletRect.h = BULLET_HEIGHT;

    bool bulletConsumed = false;

    /* Check against all enemies */
    for (int j = 0; j < enemies->count && !bulletConsumed; j++) {
      Enemy *restrict enemy = enemies->data[j];
      Entity *restrict enemyEntity = &enemy->base;

      if (!isEntityAlive(enemyEntity))
        continue;

      targetRect.x = (int)enemyEntity->pos.x;
      targetRect.y = (int)enemyEntity->pos.y;
      targetRect.w = SPRITE_WIDTH;
      targetRect.h = HUMANOID_FRAME_HEIGHT;

      if (SDL_HasIntersection(&bulletRect, &targetRect)) {
        enemyEntity->health -= 10;
        bullet->base.health = 0;
        bulletConsumed = true;

        /* Clamp health to zero */
        if (enemyEntity->health < 0)
          enemyEntity->health = 0;
      }
    }

    /* Check trap collisions for surviving bullets */
    if (!bulletConsumed && traps) {
      if (trapArrayTriggerAt(traps, bullet->base.pos.x, bullet->base.pos.y)) {
        bullet->base.health = 0;
      }
    }
  }
}

/* ---------------------------------------------------------------------------
 * Public API
 * --------------------------------------------------------------------------- */

void handleCollisions(BulletPool *restrict pool, Player *restrict player,
                      EnemyArray *restrict enemies, TrapArray *traps) {
  if (!pool || !player || !enemies)
    return;

  handleBulletEntityCollision(pool, player, enemies, traps);
  handleBulletWindowCollision(pool, player);
}

void applyPhysics(Player *restrict player, EnemyArray *restrict enemies,
                  const Map *restrict map, float dt) {
  if (!player || !map)
    return;

  applyPlayerPhysics(player, map, dt);
  applyEnemyPhysics(player, enemies, map, dt);
}
