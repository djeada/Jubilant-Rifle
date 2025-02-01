#include "entities/entity.h"

Entity *entity_create(EntityType type, float x, float y) {
  Entity *e = (Entity *)malloc(sizeof(Entity));
  if (!e)
    return NULL;
  e->type = type;
  e->pos.x = x;
  e->pos.y = y;
  e->vel.x = 0;
  e->vel.y = 0;
  e->alive = 1;
  e->health = 100;
  e->anim = NULL;
  e->update = NULL;
  return e;
}

void entity_destroy(Entity *e) { free(e); }

void common_update(Entity *self, float dt) {
  // Update position based on velocity.
  self->pos.x += self->vel.x * dt;
  self->pos.y += self->vel.y * dt;

  // Update animation (if any).
  if (self->anim) {
    self->anim->timer += dt;
    if (self->anim->timer >= self->anim->frameDuration) {
      self->anim->timer -= self->anim->frameDuration;
      self->anim->currentFrame =
          (self->anim->currentFrame + 1) % self->anim->frameCount;
    }
  }
}
