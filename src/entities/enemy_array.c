#include "entities/enemy_array.h"
#include "entities/bullet_pool.h"
#include "entities/entity.h"
#include "entities/player.h"
#include "utils/consts.h"
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------------------------------------------------------------------------
 * Constants
 * --------------------------------------------------------------------------- */

/** Initial capacity for enemy array */
#define ENEMY_ARRAY_INITIAL_CAPACITY 10

/** Enemy shooting range thresholds */
#define ENEMY_SHOOT_RANGE_X 600.0f
#define ENEMY_SHOOT_RANGE_Y 200.0f

/** Enemy shoot timer reset value */
#define ENEMY_SHOOT_COOLDOWN 2.0f

/* ---------------------------------------------------------------------------
 * Inline Helpers
 * --------------------------------------------------------------------------- */

/**
 * @brief Compute absolute value of a float.
 */
static inline float absF(float x) {
  return (x < 0.0f) ? -x : x;
}

/* ---------------------------------------------------------------------------
 * Array Operations
 * --------------------------------------------------------------------------- */

void enemyArrayInit(EnemyArray *arr) {
  if (!arr)
    return;

  arr->capacity = ENEMY_ARRAY_INITIAL_CAPACITY;
  arr->count = 0;
  arr->data = malloc(sizeof(Enemy *) * (size_t)arr->capacity);

  if (!arr->data) {
    arr->capacity = 0;
  }
}

void enemyArrayAdd(EnemyArray *arr, Enemy *enemy) {
  if (!arr || !enemy)
    return;

  /* Grow array if needed */
  if (arr->count >= arr->capacity) {
    /* Check for overflow: ensure new capacity doesn't overflow int */
    if (arr->capacity > INT_MAX / 2) {
      return; /* Would overflow, don't grow */
    }
    const int newCapacity = arr->capacity * 2;

    /* Check for size_t overflow */
    if ((size_t)newCapacity > SIZE_MAX / sizeof(Enemy *)) {
      return; /* Would overflow in malloc */
    }

    Enemy **newData = realloc(arr->data, sizeof(Enemy *) * (size_t)newCapacity);

    if (!newData)
      return; /* Allocation failed, don't add */

    arr->data = newData;
    arr->capacity = newCapacity;
  }

  arr->data[arr->count++] = enemy;
}

void enemyArrayUpdate(EnemyArray *arr, float dt, BulletPool *pool,
                      const Player *player) {
  if (!arr || !arr->data || !pool)
    return;

  for (int i = 0; i < arr->count;) {
    Enemy *restrict enemy = arr->data[i];

    if (!enemy || !isEntityAlive(&enemy->base)) {
      /* Remove dead enemy by swapping with last */
      arr->data[i] = arr->data[arr->count - 1];
      arr->count--;
      continue;
    }

    /* Update enemy logic */
    if (enemy->base.update) {
      enemy->base.update((Entity *)enemy, dt);
    }

    /* Process shooting when player is in range */
    enemy->shootTimer -= dt;

    if (enemy->shootTimer <= 0.0f && player) {
      const float dx = absF(player->base.pos.x - enemy->base.pos.x);
      const float dy = absF(player->base.pos.y - enemy->base.pos.y);

      if (dx <= ENEMY_SHOOT_RANGE_X && dy <= ENEMY_SHOOT_RANGE_Y) {
        /* Face the player */
        if (player->base.pos.x >= enemy->base.pos.x) {
          enemy->base.direction = DIRECTION_RIGHT;
        } else {
          enemy->base.direction = DIRECTION_LEFT;
        }

        /* Spawn bullet toward player */
        const float vx = (player->base.pos.x >= enemy->base.pos.x)
                             ? ENEMY_BULLET_SPEED
                             : -ENEMY_BULLET_SPEED;
        bulletPoolSpawn(pool, BULLET_SOURCE_ENEMY, enemy->base.pos.x + 20,
                        enemy->base.pos.y + 50, vx, 0.0f);

        enemy->shootTimer = ENEMY_SHOOT_COOLDOWN;
      }
    }

    i++;
  }
}

void enemyArrayDestroy(EnemyArray *arr) {
  if (!arr)
    return;

  if (arr->data) {
    for (int i = 0; i < arr->count; i++) {
      if (arr->data[i]) {
        enemyDestroy(arr->data[i]);
      }
    }
    free(arr->data);
  }

  arr->data = NULL;
  arr->count = 0;
  arr->capacity = 0;
}
