#include "entities/bullet.h"
// Initializes a Bullet object

void bulletConstructor(void *obj, ...) {
  va_list args;
  va_start(args, obj);
  bulletConstructorGeneric(obj, args);
  va_end(args);
}

void bulletConstructorGeneric(void *obj, va_list args) {
  if (!obj)
    return;
  Bullet *bullet = (Bullet *)obj;
  AnimationState animation = va_arg(args, AnimationState);
  MovementState movement = va_arg(args, MovementState);
  SDL_Texture *texture = va_arg(args, SDL_Texture *);

  bullet->animation = animation;
  bullet->movement = movement;
  bullet->texture = texture;
}

void bulletCopyConstructor(Bullet *destination, const Bullet *source) {
  if (!destination || !source)
    return;

  destination->animation = source->animation;
  destination->movement = source->movement;
  destination->texture = source->texture;
}

void bulletCopyConstructorGeneric(void *destination, void *source) {

  bulletCopyConstructor((Bullet *)destination, (Bullet *)source);
}

void bulletMove(Bullet *bullet) {
  if (!bullet)
    return;

  bullet->movement.position.x += bullet->movement.velocity.x;
  bullet->movement.position.y += bullet->movement.velocity.y;
}
