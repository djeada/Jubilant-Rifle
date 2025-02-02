#include "game/physics.h"
#include "utils/consts.h"

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
                                 EnemyArray *enemies) {
  SDL_Rect bulletRect, targetRect;

  // --- Process enemy bullets hitting the player ---
  for (int i = 0; i < pool->enemyActiveCount; i++) {
    int index = pool->enemyActiveIndices[i];
    Bullet *bullet = pool->enemyBullets[index];
    if (!bullet || !isEntityAlive(&bullet->base))
      continue;
    bulletRect.x = (int)bullet->base.pos.x;
    bulletRect.y = (int)bullet->base.pos.y;
    bulletRect.w = 10;
    bulletRect.h = 10;

    // Define player's collision box.
    targetRect.x = (int)player->base.pos.x;
    targetRect.y = (int)player->base.pos.y;
    targetRect.w = 50;
    targetRect.h = 50;

    if (SDL_HasIntersection(&bulletRect, &targetRect)) {
      player->base.health -= 10;
      bullet->base.health = 0;
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
    bulletRect.w = 10;
    bulletRect.h = 10;

    // Iterate over enemies.
    for (int j = 0; j < enemies->count; j++) {
      Entity *enemyEntity = &enemies->data[j]->base;
      if (!isEntityAlive(enemyEntity))
        continue;
      targetRect.x = (int)enemyEntity->pos.x;
      targetRect.y = (int)enemyEntity->pos.y;
      targetRect.w = 50;
      targetRect.h = 50;
      if (SDL_HasIntersection(&bulletRect, &targetRect)) {
        enemyEntity->health -= 10;
        bullet->base.health = 0;
        if (enemyEntity->health <= 0)
          enemyEntity->health = 0;
      }
    }
  }
}

void handleCollisions(BulletPool *pool, Player *player, EnemyArray *enemies) {
  handleBulletEntityCollision(pool, player, enemies);
  handleBulletWindowCollision(pool, player);
}

