#include "entities/entity.h"

/* ---------------------------------------------------------------------------
 * Entity Creation and Destruction
 * --------------------------------------------------------------------------- */

Entity *entityCreate(EntityType type, float x, float y) {
  Entity *e = (Entity *)malloc(sizeof(Entity));
  if (!e)
    return NULL;

  e->type = type;
  e->pos.x = x;
  e->pos.y = y;
  e->vel.x = 0.0f;
  e->vel.y = 0.0f;
  e->health = 100;
  e->direction = DIRECTION_RIGHT;
  e->anim = NULL;
  e->update = NULL;

  return e;
}

void entityDestroy(Entity *e) {
  if (!e)
    return;

  if (e->anim) {
    free(e->anim);
    e->anim = NULL;
  }
  free(e);
}

/* ---------------------------------------------------------------------------
 * Entity Update
 * --------------------------------------------------------------------------- */

void commonEntityUpdate(Entity *self, float dt) {
  if (!self || dt < 0.0f)
    return;

  /* Update position based on velocity */
  self->pos.x += self->vel.x * dt;
  self->pos.y += self->vel.y * dt;

  /* Update animation if present */
  Animation *restrict anim = self->anim;
  if (anim && anim->frameCount > 0 && anim->frameDuration > 0.0f) {
    anim->timer += dt;
    if (anim->timer >= anim->frameDuration) {
      anim->timer -= anim->frameDuration;
      anim->currentFrame = (anim->currentFrame + 1) % anim->frameCount;
    }
  }
}

/* ---------------------------------------------------------------------------
 * Entity Queries
 * --------------------------------------------------------------------------- */

bool isEntityAlive(const Entity *e) {
  return e != NULL && e->health > 0;
}

bool isEntityMoving(const Entity *e) {
  if (!e)
    return false;
  return e->vel.x != 0.0f || e->vel.y != 0.0f;
}

SDL_Rect entityGetBounds(const Entity *e, int width, int height) {
  SDL_Rect rect = {0, 0, 0, 0};
  if (e) {
    rect.x = (int)e->pos.x;
    rect.y = (int)e->pos.y;
    rect.w = width;
    rect.h = height;
  }
  return rect;
}

