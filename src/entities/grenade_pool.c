#include "entities/grenade_pool.h"
#include <math.h>
#include <stdlib.h>

#define GRENADE_THROW_SPEED 300.0f
#define GRENADE_THROW_ANGLE 45.0f /* degrees */

void grenadePoolInit(GrenadePool *pool) {
  if (!pool)
    return;

  for (int i = 0; i < MAX_GRENADES; i++) {
    pool->grenades[i] = NULL;
  }
  pool->activeCount = 0;
}

void grenadePoolDestroy(GrenadePool *pool) {
  if (!pool)
    return;

  for (int i = 0; i < MAX_GRENADES; i++) {
    if (pool->grenades[i]) {
      grenadeDestroy(pool->grenades[i]);
      pool->grenades[i] = NULL;
    }
  }
  pool->activeCount = 0;
}

int grenadePoolAdd(GrenadePool *pool, Grenade *grenade) {
  if (!pool || !grenade)
    return -1;

  for (int i = 0; i < MAX_GRENADES; i++) {
    if (pool->grenades[i] == NULL) {
      pool->grenades[i] = grenade;
      pool->activeCount++;
      return 0;
    }
  }
  return -1; /* Pool is full */
}

void grenadePoolUpdate(GrenadePool *pool, float dt) {
  if (!pool)
    return;

  for (int i = 0; i < MAX_GRENADES; i++) {
    if (pool->grenades[i]) {
      grenadeUpdate(pool->grenades[i], dt);
    }
  }

  grenadePoolCleanup(pool);
}

void grenadePoolCleanup(GrenadePool *pool) {
  if (!pool)
    return;

  for (int i = 0; i < MAX_GRENADES; i++) {
    if (pool->grenades[i] && !isGrenadeActive(pool->grenades[i])) {
      grenadeDestroy(pool->grenades[i]);
      pool->grenades[i] = NULL;
      pool->activeCount--;
    }
  }
}

void grenadePoolThrow(GrenadePool *pool, GrenadeSource source,
                       float x, float y, float targetX, float targetY) {
  if (!pool)
    return;

  /* Calculate throw direction */
  float dx = targetX - x;
  float dy = targetY - y;
  float distance = sqrtf(dx * dx + dy * dy);
  
  if (distance < 1.0f)
    return;

  /* Normalize and apply throw speed with arc */
  float angle = GRENADE_THROW_ANGLE * 3.14159f / 180.0f;
  float vx = (dx / distance) * GRENADE_THROW_SPEED;
  float vy = -GRENADE_THROW_SPEED * sinf(angle); /* Negative for upward arc */
  
  Grenade *grenade = grenadeCreate(source, x, y, vx, vy);
  if (grenade) {
    grenadePoolAdd(pool, grenade);
  }
}
