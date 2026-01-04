#include "entities/bullet.h"
#include "utils/consts.h"

#include <math.h>
#include <stdlib.h>

/* ---------------------------------------------------------------------------
 * Constants
 * --------------------------------------------------------------------------- */

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ---------------------------------------------------------------------------
 * Bullet Creation
 * --------------------------------------------------------------------------- */

Bullet *bulletCreate(BulletSource source, float x, float y, float vx,
                     float vy) {
  Bullet *bullet = malloc(sizeof(Bullet));
  if (!bullet)
    return NULL;

  bullet->base.type = ENTITY_BULLET;
  bullet->base.pos.x = x;
  bullet->base.pos.y = y;
  bullet->base.vel.x = vx;
  bullet->base.vel.y = vy;
  bullet->base.health = 1;
  bullet->base.direction = (vx >= 0.0f) ? DIRECTION_RIGHT : DIRECTION_LEFT;
  bullet->base.anim = NULL;
  bullet->base.update = (void (*)(Entity *, float))bulletUpdate;

  bullet->source = source;

  /* Calculate rotation angle from velocity (in degrees) */
  bullet->rotation = atan2f(vy, vx) * (180.0f / (float)M_PI);

  return bullet;
}

/* ---------------------------------------------------------------------------
 * Bullet Update
 * --------------------------------------------------------------------------- */

void bulletUpdate(Bullet *bullet, float dt) {
  if (!bullet || dt < 0.0f)
    return;

  commonEntityUpdate(&bullet->base, dt);
}

/* ---------------------------------------------------------------------------
 * Bullet Queries
 * --------------------------------------------------------------------------- */

bool isBulletAlive(const Bullet *bullet) {
  return bullet != NULL && isEntityAlive(&bullet->base);
}

float bulletGetRotation(const Bullet *bullet) {
  if (!bullet)
    return 0.0f;
  return bullet->rotation;
}
