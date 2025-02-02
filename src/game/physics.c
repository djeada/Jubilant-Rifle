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

void handleBulletEntityCollision(BulletPool *pool, Entity *player,
                                 EnemyArray *enemies) {
  for (int i = 0; i < pool->activeCount; i++) {
    int bulletIndex = pool->activeIndices[i];
    Entity *bullet = pool->bullets[bulletIndex];
    if (!bullet || !isEntityAlive(bullet))
      continue;

    SDL_Rect bulletRect = {(int)bullet->pos.x, (int)bullet->pos.y, 10, 10};

    if (bullet->vel.y > 0) { // Enemy bullet colliding with the player.
      SDL_Rect playerRect = {(int)player->pos.x, (int)player->pos.y, 50, 50};
      if (SDL_HasIntersection(&bulletRect, &playerRect)) {
        player->health -= 10; // Apply damage to the player.
        bullet->health = 0;   // Mark the bullet as destroyed.
      }
    } else if (bullet->vel.y < 0) { // Player bullet colliding with enemies.
      for (int j = 0; j < enemies->count; j++) {
        Entity *enemyEntity = (Entity *)&enemies->data[j]->base;
        if (!isEntityAlive(enemyEntity))
          continue;

        SDL_Rect enemyRect = {(int)enemyEntity->pos.x, (int)enemyEntity->pos.y,
                              50, 50};
        if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
          enemyEntity->health -= 10; // Apply damage to the enemy.
          bullet->health = 0;        // Destroy the bullet.
          if (enemyEntity->health <= 0)
            enemyEntity->health = 0; // Optionally mark enemy as "destroyed".
        }
      }
    }
  }
}

void handleCollisions(BulletPool *pool, Entity *player, EnemyArray *enemies) {
  handleBulletEntityCollision(pool, player, enemies);
  handleBulletWindowCollision(pool);
}
