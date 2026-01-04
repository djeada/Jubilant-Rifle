#include "entities/grenade.h"
#include "utils/consts.h"
#include <stdlib.h>

/* Grenade constants */
#define GRENADE_FUSE_TIME 2.0f        /* Seconds until explosion */
#define GRENADE_EXPLOSION_DURATION 0.3f /* How long explosion lasts */
#define GRENADE_EXPLOSION_DAMAGE 50
#define GRENADE_GRAVITY 400.0f
#define GRENADE_BOUNCE_FACTOR 0.5f

Grenade *grenadeCreate(GrenadeSource source, float x, float y, float vx, float vy) {
  Grenade *grenade = malloc(sizeof(Grenade));
  if (!grenade)
    return NULL;

  grenade->base.type = ENTITY_BULLET; /* Reuse bullet type for simplicity */
  grenade->base.pos.x = x;
  grenade->base.pos.y = y;
  grenade->base.vel.x = vx;
  grenade->base.vel.y = vy;
  grenade->base.health = 1;
  grenade->base.direction = (vx >= 0) ? DIRECTION_RIGHT : DIRECTION_LEFT;
  grenade->base.anim = NULL;
  grenade->base.update = NULL;

  grenade->source = source;
  grenade->state = GRENADE_STATE_FLYING;
  grenade->fuseTimer = GRENADE_FUSE_TIME;
  grenade->explosionRadius = GRENADE_EXPLOSION_RADIUS;
  grenade->explosionDamage = GRENADE_EXPLOSION_DAMAGE;
  grenade->explosionTimer = 0.0f;

  return grenade;
}

void grenadeDestroy(Grenade *grenade) {
  if (grenade) {
    free(grenade);
  }
}

void grenadeUpdate(Grenade *grenade, float dt) {
  if (!grenade)
    return;

  switch (grenade->state) {
  case GRENADE_STATE_FLYING:
  case GRENADE_STATE_BOUNCING:
    /* Apply gravity */
    grenade->base.vel.y += GRENADE_GRAVITY * dt;
    
    /* Update position */
    grenade->base.pos.x += grenade->base.vel.x * dt;
    grenade->base.pos.y += grenade->base.vel.y * dt;
    
    /* Update fuse timer */
    grenade->fuseTimer -= dt;
    if (grenade->fuseTimer <= 0) {
      grenade->state = GRENADE_STATE_EXPLODING;
      grenade->explosionTimer = GRENADE_EXPLOSION_DURATION;
    }
    break;

  case GRENADE_STATE_EXPLODING:
    grenade->explosionTimer -= dt;
    if (grenade->explosionTimer <= 0) {
      grenade->state = GRENADE_STATE_DONE;
    }
    break;

  case GRENADE_STATE_DONE:
    /* Do nothing */
    break;
  }
}

bool isGrenadeActive(const Grenade *grenade) {
  if (!grenade)
    return false;
  return grenade->state != GRENADE_STATE_DONE;
}

bool isGrenadeExploding(const Grenade *grenade) {
  if (!grenade)
    return false;
  return grenade->state == GRENADE_STATE_EXPLODING;
}

SDL_Rect grenadeGetExplosionBounds(const Grenade *grenade) {
  SDL_Rect rect = {0, 0, 0, 0};
  if (!grenade || grenade->state != GRENADE_STATE_EXPLODING)
    return rect;

  int radius = (int)grenade->explosionRadius;
  rect.x = (int)grenade->base.pos.x - radius;
  rect.y = (int)grenade->base.pos.y - radius;
  rect.w = radius * 2;
  rect.h = radius * 2;
  return rect;
}
