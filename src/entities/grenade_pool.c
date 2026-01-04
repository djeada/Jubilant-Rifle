#include "entities/grenade_pool.h"
#include <math.h>
#include <stdlib.h>

/* ---------------------------------------------------------------------------
 * Constants
 * --------------------------------------------------------------------------- */

#define GRENADE_THROW_SPEED 300.0f
#define GRENADE_THROW_ANGLE_DEG 45.0f
#define GRENADE_MIN_THROW_DISTANCE 1.0f

/* Pre-computed angle in radians */
static const float GRENADE_THROW_ANGLE_RAD =
    GRENADE_THROW_ANGLE_DEG * 3.14159f / 180.0f;

/* ---------------------------------------------------------------------------
 * Pool Lifecycle
 * --------------------------------------------------------------------------- */

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

/* ---------------------------------------------------------------------------
 * Pool Operations
 * --------------------------------------------------------------------------- */

int grenadePoolAdd(GrenadePool *pool, Grenade *grenade) {
  if (!pool || !grenade)
    return -1;

  /* Find first available slot */
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
  if (!pool || dt < 0.0f)
    return;

  /* Update all active grenades */
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
    Grenade *grenade = pool->grenades[i];

    if (grenade && !isGrenadeActive(grenade)) {
      grenadeDestroy(grenade);
      pool->grenades[i] = NULL;
      pool->activeCount--;
    }
  }
}

void grenadePoolThrow(GrenadePool *pool, GrenadeSource source, float x, float y,
                      float targetX, float targetY) {
  if (!pool)
    return;

  /* Calculate throw direction */
  const float dx = targetX - x;
  const float dy = targetY - y;
  const float distance = sqrtf(dx * dx + dy * dy);

  if (distance < GRENADE_MIN_THROW_DISTANCE)
    return;

  /* Calculate velocity with arc */
  const float vx = (dx / distance) * GRENADE_THROW_SPEED;
  const float vy = -GRENADE_THROW_SPEED * sinf(GRENADE_THROW_ANGLE_RAD);

  Grenade *grenade = grenadeCreate(source, x, y, vx, vy);
  if (grenade) {
    if (grenadePoolAdd(pool, grenade) != 0) {
      /* Pool was full, clean up */
      grenadeDestroy(grenade);
    }
  }
}
