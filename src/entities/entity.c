#include "entities/entity.h"

Entity *entityCreate(EntityType type, float x, float y) {
  Entity *e = (Entity *)malloc(sizeof(Entity));
  if (!e)
    return NULL;
  e->type = type;
  e->pos.x = x;
  e->pos.y = y;
  e->vel.x = 0;
  e->vel.y = 0;
  e->health = 100;
  e->direction = DIRECTION_RIGHT;
  e->anim = NULL;
  e->update = NULL;
  return e;
}

void entityDestroy(Entity *e) {
  if (e) {
    if (e->anim) {
      free(e->anim);
    }
    free(e);
  }
}

void commonEntityUpdate(Entity *self, float dt) {
  if (!self)
    return;

  /* Update position based on velocity. */
  self->pos.x += self->vel.x * dt;
  self->pos.y += self->vel.y * dt;

  /* Update animation (if any). */
  if (self->anim && self->anim->frameCount > 0) {
    self->anim->timer += dt;
    if (self->anim->timer >= self->anim->frameDuration) {
      self->anim->timer -= self->anim->frameDuration;
      self->anim->currentFrame =
          (self->anim->currentFrame + 1) % self->anim->frameCount;
    }
  }
}

bool isEntityAlive(const Entity *e) { return e && e->health > 0; }

bool isEntityMoving(const Entity *e) {
  if (!e)
    return false;
  return (e->vel.x != 0 || e->vel.y != 0);
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

