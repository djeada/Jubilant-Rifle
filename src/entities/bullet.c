#include "entities/bullet.h"
#include "utils/consts.h"

#include <stdlib.h>

// Create a new bullet, initializing the base entity values and setting the
// source.
Bullet *bulletCreate(BulletSource source, float x, float y, float vx,
                     float vy) {
  Bullet *bullet = malloc(sizeof(Bullet));
  if (!bullet)
    return NULL; // Check malloc result if desired

  bullet->base.type = ENTITY_BULLET;
  bullet->base.pos.x = x;
  bullet->base.pos.y = y;
  bullet->base.vel.x = vx;
  bullet->base.vel.y = vy;
  bullet->base.health = 1;
  bullet->base.anim = NULL;
  // Set the update function pointer. Note that we cast bulletUpdate to match
  // the signature:
  bullet->base.update = (void (*)(Entity *, float))bulletUpdate;

  bullet->source = source;
  return bullet;
}

// Update the bullet by updating its position and checking bounds.
void bulletUpdate(Bullet *bullet, float dt) {
  // Update the entity’s position and timer etc.
  commonEntityUpdate(&bullet->base, dt);
}

bool isBulletAlive(const Bullet *bullet) {
  return isEntityAlive(&bullet->base);
}

