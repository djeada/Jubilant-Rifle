#include "game/physics.h"
#include "utils/consts.h"

// todo
// - gravity
// - collisions only on subset of enemies

void handleBulletWindowCollision(BulletPool *pool) {
  for (int i = 0; i < pool->activeCount; i++) {
    int bulletIndex = pool->activeIndices[i];
    Entity *bullet = pool->bullets[bulletIndex];
    if (!bullet || !isEntityAlive(bullet))
      continue;

    // Check if the bullet's position is outside the game bounds.
    if (bullet->pos.x < 0 || bullet->pos.x > GAME_WIDTH || bullet->pos.y < 0 ||
        bullet->pos.y > GAME_HEIGHT) {
      bullet->health = 0; // Mark bullet as dead/inactive.
    }
  }
}

void handleBulletEntityCollision(BulletPool *pool, Player *player,
                                 EnemyArray *enemies) {
  for (int i = 0; i < pool->activeCount; i++) {
    int bulletIndex = pool->activeIndices[i];
    Entity *bullet = pool->bullets[bulletIndex];
    if (!bullet || !isEntityAlive(bullet))
      continue;
    SDL_Rect bulletRect = {(int)bullet->pos.x, (int)bullet->pos.y, 10, 10};
    if (bullet->vel.y > 0) {
      SDL_Rect playerRect = {(int)player->base.pos.x, (int)player->base.pos.y,
                             50, 50};
      if (SDL_HasIntersection(&bulletRect, &playerRect)) {
        player->base.health -= 10;
        bullet->health = 0;
      }
    } else if (bullet->vel.y < 0) {
      for (int j = 0; j < enemies->count; j++) {
        Entity *enemyEntity = &enemies->data[j]->base;
        if (!isEntityAlive(enemyEntity))
          continue;
        SDL_Rect enemyRect = {(int)enemyEntity->pos.x, (int)enemyEntity->pos.y,
                              50, 50};
        if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
          enemyEntity->health -= 10;
          bullet->health = 0;
          if (enemyEntity->health <= 0)
            enemyEntity->health = 0;
        }
      }
    }
  }
}

void handleCollisions(BulletPool *pool, Player *player, EnemyArray *enemies) {
  handleBulletEntityCollision(pool, player, enemies);
  handleBulletWindowCollision(pool);
}
