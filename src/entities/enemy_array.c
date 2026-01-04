#include "entities/enemy_array.h"
#include "entities/bullet_pool.h"
#include "entities/entity.h"
#include "entities/player.h"
#include "utils/consts.h"
#include <stdlib.h>

void enemyArrayInit(EnemyArray *arr) {
  arr->capacity = 10;
  arr->count = 0;
  arr->data = malloc(sizeof(Enemy *) * arr->capacity);
}

void enemyArrayAdd(EnemyArray *arr, Enemy *enemy) {
  if (arr->count >= arr->capacity) {
    arr->capacity *= 2;
    arr->data = realloc(arr->data, sizeof(Enemy *) * arr->capacity);
  }
  arr->data[arr->count++] = enemy;
}

/*
 * Update each enemy and remove dead ones from the array.
 * This prevents dead enemies from updating or spawning bullets.
 */
void enemyArrayUpdate(EnemyArray *arr, float dt, BulletPool *pool,
                      const Player *player) {
  for (int i = 0; i < arr->count;) {
    Enemy *enemy = arr->data[i];

    if (!isEntityAlive(&enemy->base)) {
      // Remove enemy by replacing it with the last one.
      arr->data[i] = arr->data[arr->count - 1];
      arr->count--;
      continue; // Process the swapped enemy at index i.
    }

    // Update enemy logic.
    enemy->base.update((Entity *)enemy, dt);

    // Process shooting only when player is in a reasonable line-of-sight band.
    enemy->shootTimer -= dt;
    if (enemy->shootTimer <= 0 && player) {
      float dx = player->base.pos.x - enemy->base.pos.x;
      float dy = player->base.pos.y - enemy->base.pos.y;
      if (dx < 0)
        dx = -dx;
      if (dy < 0)
        dy = -dy;

      if (dx <= 600.0f && dy <= 200.0f) {
        if (player->base.pos.x >= enemy->base.pos.x) {
          enemy->base.direction = DIRECTION_RIGHT;
        } else {
          enemy->base.direction = DIRECTION_LEFT;
        }
        float vx = (player->base.pos.x >= enemy->base.pos.x)
                       ? ENEMY_BULLET_SPEED
                       : -ENEMY_BULLET_SPEED;
        bulletPoolSpawn(pool, BULLET_SOURCE_ENEMY, enemy->base.pos.x + 20,
                        enemy->base.pos.y + 50, vx, 0);
        enemy->shootTimer = 2.0f;
      }
    }
    i++;
  }
}

void enemyArrayDestroy(EnemyArray *arr) {
  if (!arr)
    return;
  for (int i = 0; i < arr->count; i++) {
    if (arr->data[i])
      enemyDestroy(arr->data[i]);
  }
  free(arr->data);
  arr->data = NULL;
  arr->count = 0;
  arr->capacity = 0;
}
