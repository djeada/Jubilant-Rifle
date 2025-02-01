#include "game/physics.h"

void handleCollisions(BulletPool *pool, Entity *player, EnemyArray *enemies) {
  for (int i = 0; i < BULLET_POOL_CAPACITY; i++) {
    Entity *bullet = pool->bullets[i];
    if (!bullet || !isEntityAlive(bullet)) // Use isEntityAlive here
      continue;

    SDL_Rect bulletRect = {(int)bullet->pos.x, (int)bullet->pos.y, 10, 10};

    // Use bullet's vertical velocity to determine its origin.
    if (bullet->vel.y > 0) { // enemy bullet
      SDL_Rect playerRect = {(int)player->pos.x, (int)player->pos.y, 50, 50};
      if (SDL_HasIntersection(&bulletRect, &playerRect)) {
        player->health -= 10; // Adjust damage as needed.
        bullet->health =
            0; // Instead of alive, use health property to deactivate
      }
    } else if (bullet->vel.y < 0) { // player bullet
      for (int j = 0; j < enemies->count; j++) {
        Entity *enemyEntity = (Entity *)&enemies->data[j]->base;
        if (!isEntityAlive(enemyEntity)) // Use isEntityAlive here
          continue;

        SDL_Rect enemyRect = {(int)enemyEntity->pos.x, (int)enemyEntity->pos.y,
                              50, 50};
        if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
          enemyEntity->health -= 10;
          bullet->health = 0; // Bullet is destroyed
          if (enemyEntity->health <= 0)
            enemyEntity->health = 0; // Mark enemy as "destroyed"
        }
      }
    }
  }
}