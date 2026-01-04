#include "game/physics.h"
#include "entities/enemy.h"
#include "map/ladder.h"
#include "utils/consts.h"
#include <limits.h>

static const Platform *findSupportingPlatform(const Entity *entity,
                                              const Map *map, int width,
                                              int height) {
  if (!entity || !map || !map->platforms)
    return NULL;

  SDL_Rect rect = {(int)entity->pos.x, (int)entity->pos.y, width, height};
  const Platform *best = NULL;
  int bestTop = INT_MAX;

  for (size_t i = 0; i < map->platformCount; i++) {
    const Platform *platform = &map->platforms[i];
    int rectRight = rect.x + rect.w;
    int platformRight = platform->x + platform->w;

    if (rectRight <= platform->x || rect.x >= platformRight)
      continue;

    int rectBottom = rect.y + rect.h;
    if (rectBottom >= platform->y && rectBottom <= platform->y + platform->h) {
      if (platform->y < bestTop) {
        bestTop = platform->y;
        best = platform;
      }
    }
  }

  return best;
}

static const Platform *resolvePlatformCollision(Entity *entity, const Map *map,
                                                int width, int height) {
  if (!entity || !map)
    return NULL;

  const Platform *platform =
      (entity->vel.y >= 0) ? findSupportingPlatform(entity, map, width, height)
                           : NULL;

  if (platform) {
    entity->pos.y = (float)(platform->y - height);
    entity->vel.y = 0;
  } else if (map->rect.h > 0) {
    float maxY = (float)(map->rect.h - height);
    if (entity->pos.y > maxY) {
      entity->pos.y = maxY;
      entity->vel.y = 0;
    }
  }

  return platform;
}

static void applyPlayerPhysics(Player *player, const Map *map, float dt) {
  if (!player || !map)
    return;

  const Ladder *ladder = NULL;
  if (map->ladders) {
    LadderArray *ladders = (LadderArray *)map->ladders;
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

  if (ladder && (player->climbIntentUp || player->climbIntentDown)) {
    bool wasClimbing = player->isClimbing;
    player->isClimbing = true;
    player->base.vel.x = 0;
    if (!wasClimbing) {
      player->base.pos.x =
          (float)(ladder->rect.x + (ladder->rect.w - SPRITE_WIDTH) / 2);
    }
    if (player->climbIntentUp) {
      player->base.vel.y = -LADDER_CLIMB_SPEED;
    } else {
      player->base.vel.y = LADDER_CLIMB_SPEED;
    }
  } else if (!ladder) {
    player->isClimbing = false;
  }

  if (player->isClimbing) {
    if (!player->climbIntentUp && !player->climbIntentDown) {
      player->base.vel.y = 0;
    }
  } else {
    if (player->jumpRequested && player->jumpsUsed < 2) {
      player->base.vel.y = -PLAYER_JUMP_SPEED;
      player->jumpsUsed++;
    }
    player->base.vel.y += PLAYER_FALL_INCREMENT * dt;
  }
  player->jumpRequested = false;

  commonEntityUpdate(&player->base, dt);
  resolvePlatformCollision(&player->base, map, SPRITE_WIDTH,
                           HUMANOID_FRAME_HEIGHT);
  if (!player->isClimbing && player->base.vel.y == 0) {
    player->jumpsUsed = 0;
  }
}

static void applyEnemyPhysics(Player *player, EnemyArray *enemies,
                              const Map *map, float dt) {
  if (!enemies || !map)
    return;

  for (int i = 0; i < enemies->count; i++) {
    Enemy *enemy = enemies->data[i];
    if (!enemy || !isEnemyAlive(enemy))
      continue;

    if (enemy->state == ENEMY_STATE_PARACHUTING) {
      commonEntityUpdate(&enemy->base, dt);
      continue;
    }

    if (enemy->state != ENEMY_STATE_LANDING) {
      enemy->base.vel.y += ENEMY_FALL_INCREMENT * dt;
    } else {
      enemy->base.vel.x = 0;
    }

    commonEntityUpdate(&enemy->base, dt);
    const Platform *platform = resolvePlatformCollision(
        &enemy->base, map, SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT);

    if (enemy->state == ENEMY_STATE_NORMAL && platform) {
      float speed = ENEMY_MOVE_SPEED;
      bool chasing = false;
      if (player) {
        float dx = player->base.pos.x - enemy->base.pos.x;
        float dy = player->base.pos.y - enemy->base.pos.y;
        if (dx < 0)
          dx = -dx;
        if (dy < 0)
          dy = -dy;
        chasing = (dx <= 400.0f && dy <= 120.0f);
        if (chasing) {
          if (player->base.pos.x > enemy->base.pos.x) {
            enemySetHorizontalVelocity(enemy, speed);
          } else {
            enemySetHorizontalVelocity(enemy, -speed);
          }
        }
      }

      if (!chasing && enemy->base.vel.x == 0) {
        if (enemy->base.direction == DIRECTION_RIGHT) {
          enemySetHorizontalVelocity(enemy, speed);
        } else {
          enemySetHorizontalVelocity(enemy, -speed);
        }
      }

      float leftEdge = (float)platform->x;
      float rightEdge = (float)(platform->x + platform->w - SPRITE_WIDTH);

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

void handleBulletWindowCollision(BulletPool *pool, Player *player) {
  // Define the culling boundaries relative to the player.
  float leftBound = player->base.pos.x - GAME_WIDTH;
  float rightBound = player->base.pos.x + GAME_WIDTH;
  float topBound = player->base.pos.y - GAME_HEIGHT;
  float bottomBound = player->base.pos.y + GAME_HEIGHT;

  // Check player bullets.
  for (int i = 0; i < pool->playerActiveCount; i++) {
    int index = pool->playerActiveIndices[i];
    Bullet *bullet = pool->playerBullets[index];
    if (!bullet || !isEntityAlive(&bullet->base))
      continue;

    // If the bullet is outside the allowed range from the player, mark it as
    // inactive.
    if (bullet->base.pos.x < leftBound || bullet->base.pos.x > rightBound ||
        bullet->base.pos.y < topBound || bullet->base.pos.y > bottomBound) {
      bullet->base.health = 0;
    }
  }

  // Check enemy bullets.
  for (int i = 0; i < pool->enemyActiveCount; i++) {
    int index = pool->enemyActiveIndices[i];
    Bullet *bullet = pool->enemyBullets[index];
    if (!bullet || !isEntityAlive(&bullet->base))
      continue;

    // Use the same bounds for enemy bullets.
    if (bullet->base.pos.x < leftBound || bullet->base.pos.x > rightBound ||
        bullet->base.pos.y < topBound || bullet->base.pos.y > bottomBound) {
      bullet->base.health = 0;
    }
  }
}

void handleBulletEntityCollision(BulletPool *pool, Player *player,
                                 EnemyArray *enemies, TrapArray *traps) {
  SDL_Rect bulletRect, targetRect;

  // --- Process enemy bullets hitting the player ---
  for (int i = 0; i < pool->enemyActiveCount; i++) {
    int index = pool->enemyActiveIndices[i];
    Bullet *bullet = pool->enemyBullets[index];
    if (!bullet || !isEntityAlive(&bullet->base))
      continue;
    bulletRect.x = (int)bullet->base.pos.x;
    bulletRect.y = (int)bullet->base.pos.y;
    bulletRect.w = BULLET_WIDTH;
    bulletRect.h = BULLET_HEIGHT;

    // Define player's collision box.
    targetRect.x = (int)player->base.pos.x;
    targetRect.y = (int)player->base.pos.y;
    targetRect.w = SPRITE_WIDTH;
    targetRect.h = HUMANOID_FRAME_HEIGHT;

    if (SDL_HasIntersection(&bulletRect, &targetRect)) {
      player->base.health -= 10;
      bullet->base.health = 0;
    }
    if (traps && bullet->base.health > 0) {
      if (trapArrayTriggerAt(traps, bullet->base.pos.x, bullet->base.pos.y)) {
        bullet->base.health = 0;
      }
    }
  }

  // --- Process player bullets hitting enemies ---
  for (int i = 0; i < pool->playerActiveCount; i++) {
    int index = pool->playerActiveIndices[i];
    Bullet *bullet = pool->playerBullets[index];
    if (!bullet || !isEntityAlive(&bullet->base))
      continue;
    bulletRect.x = (int)bullet->base.pos.x;
    bulletRect.y = (int)bullet->base.pos.y;
    bulletRect.w = BULLET_WIDTH;
    bulletRect.h = BULLET_HEIGHT;

    // Iterate over enemies.
    for (int j = 0; j < enemies->count; j++) {
      Entity *enemyEntity = &enemies->data[j]->base;
      if (!isEntityAlive(enemyEntity))
        continue;
      targetRect.x = (int)enemyEntity->pos.x;
      targetRect.y = (int)enemyEntity->pos.y;
      targetRect.w = SPRITE_WIDTH;
      targetRect.h = HUMANOID_FRAME_HEIGHT;
      if (SDL_HasIntersection(&bulletRect, &targetRect)) {
        enemyEntity->health -= 10;
        bullet->base.health = 0;
        if (enemyEntity->health <= 0)
          enemyEntity->health = 0;
      }
    }
    if (traps && bullet->base.health > 0) {
      if (trapArrayTriggerAt(traps, bullet->base.pos.x, bullet->base.pos.y)) {
        bullet->base.health = 0;
      }
    }
  }
}

void handleCollisions(BulletPool *pool, Player *player, EnemyArray *enemies,
                      TrapArray *traps) {
  handleBulletEntityCollision(pool, player, enemies, traps);
  handleBulletWindowCollision(pool, player);
}

void applyPhysics(Player *player, EnemyArray *enemies, const Map *map,
                  float dt) {
  applyPlayerPhysics(player, map, dt);
  applyEnemyPhysics(player, enemies, map, dt);
}
