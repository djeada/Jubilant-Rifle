#include "entities/grenade.h"
#include "utils/consts.h"
#include <stdlib.h>

/* ---------------------------------------------------------------------------
 * Grenade Constants
 * --------------------------------------------------------------------------- */

#define GRENADE_FUSE_TIME_DEFAULT 2.0f
#define GRENADE_EXPLOSION_DURATION 0.3f
#define GRENADE_EXPLOSION_DAMAGE_VALUE 50
#define GRENADE_GRAVITY 400.0f
#define GRENADE_BOUNCE_FACTOR 0.5f

/* ---------------------------------------------------------------------------
 * Grenade Creation and Destruction
 * --------------------------------------------------------------------------- */

Grenade *grenadeCreate(GrenadeSource source, float x, float y, float vx,
                       float vy) {
  Grenade *grenade = malloc(sizeof(Grenade));
  if (!grenade)
    return NULL;

  grenade->base.type = ENTITY_BULLET;
  grenade->base.pos.x = x;
  grenade->base.pos.y = y;
  grenade->base.vel.x = vx;
  grenade->base.vel.y = vy;
  grenade->base.health = 1;
  grenade->base.direction = (vx >= 0.0f) ? DIRECTION_RIGHT : DIRECTION_LEFT;
  grenade->base.anim = NULL;
  grenade->base.update = NULL;

  grenade->source = source;
  grenade->state = GRENADE_STATE_FLYING;
  grenade->fuseTimer = GRENADE_FUSE_TIME_DEFAULT;
  grenade->explosionRadius = (float)GRENADE_EXPLOSION_RADIUS;
  grenade->explosionDamage = GRENADE_EXPLOSION_DAMAGE_VALUE;
  grenade->explosionTimer = 0.0f;
  grenade->rotation = 0.0f;

  return grenade;
}

void grenadeDestroy(Grenade *grenade) {
  if (grenade) {
    free(grenade);
  }
}

/* ---------------------------------------------------------------------------
 * Grenade Update
 * --------------------------------------------------------------------------- */

void grenadeUpdate(Grenade *grenade, float dt) {
  if (!grenade || dt < 0.0f)
    return;

  switch (grenade->state) {
  case GRENADE_STATE_FLYING:
  case GRENADE_STATE_BOUNCING:
    /* Apply gravity */
    grenade->base.vel.y += GRENADE_GRAVITY * dt;

    /* Update position */
    grenade->base.pos.x += grenade->base.vel.x * dt;
    grenade->base.pos.y += grenade->base.vel.y * dt;

    /* Animate rotation based on velocity */
    grenade->rotation +=
        (grenade->base.vel.x * 2.0f + grenade->base.vel.y) * dt;

    /* Update fuse timer */
    grenade->fuseTimer -= dt;
    if (grenade->fuseTimer <= 0.0f) {
      grenade->state = GRENADE_STATE_EXPLODING;
      grenade->explosionTimer = GRENADE_EXPLOSION_DURATION;
    }
    break;

  case GRENADE_STATE_EXPLODING:
    grenade->explosionTimer -= dt;
    if (grenade->explosionTimer <= 0.0f) {
      grenade->state = GRENADE_STATE_DONE;
    }
    break;

  case GRENADE_STATE_DONE:
    /* No update needed */
    break;
  }
}

/* ---------------------------------------------------------------------------
 * Grenade Queries
 * --------------------------------------------------------------------------- */

bool isGrenadeActive(const Grenade *grenade) {
  return grenade != NULL && grenade->state != GRENADE_STATE_DONE;
}

bool isGrenadeExploding(const Grenade *grenade) {
  return grenade != NULL && grenade->state == GRENADE_STATE_EXPLODING;
}

SDL_Rect grenadeGetExplosionBounds(const Grenade *grenade) {
  SDL_Rect rect = {0, 0, 0, 0};

  if (!grenade || grenade->state != GRENADE_STATE_EXPLODING)
    return rect;

  const int radius = (int)grenade->explosionRadius;
  rect.x = (int)grenade->base.pos.x - radius;
  rect.y = (int)grenade->base.pos.y - radius;
  rect.w = radius * 2;
  rect.h = radius * 2;

  return rect;
}
