#include "entities/bullet.h"
// Initializes a Bullet object

void bulletConstructor(void *obj, ...) {
  va_list args;
  va_start(args, obj); // Initialize va_list with the last known fixed argument

  bulletConstructorVariadic(obj,
                            args); // Pass the va_list to the actual constructor

  va_end(args); // Clean up va_list
}

void bulletConstructorVariadic(void *obj, va_list args) {
  if (!obj)
    return;
  Bullet *bullet = (Bullet *)obj;
  AnimationState animation = va_arg(
      args, AnimationState); // Assuming AnimationState can be passed this way
  MovementState movement = va_arg(
      args, MovementState); // Assuming MovementState can be passed this way
  SDL_Texture *texture = va_arg(args, SDL_Texture *);

  bullet->animation = animation;
  bullet->movement = movement;
  bullet->texture = texture;
}

// Copies the content of a Bullet object from source to destination
void bulletCopy(Bullet *destination, const Bullet *source) {
  if (!destination || !source)
    return;

  destination->animation = source->animation;
  destination->movement = source->movement;
  // For the texture, you might want to increase a reference count if that's how
  // you're managing textures, or you might want to actually copy the texture.
  // For simplicity, we're just copying the reference.
  destination->texture = source->texture;
}

void bulletMove(Bullet *bullet) {
  if (!bullet)
    return;

  // Update the bullet's position based on its velocity
  bullet->movement.position.x += bullet->movement.velocity.x;
  bullet->movement.position.y += bullet->movement.velocity.y;
}
