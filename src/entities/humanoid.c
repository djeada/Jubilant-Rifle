#include "entities/humanoid.h"
#include "utils/consts.h"
#include "utils/resources.h"

void humanoidConstructor(void *obj, ...) {
  if (!obj)
    return;

  va_list args;
  va_start(args, obj); // Initialize va_list

  Humanoid *humanoid = (Humanoid *)obj;
  humanoid->animation = va_arg(args, AnimationState); // Retrieve AnimationState
  humanoid->movement = va_arg(args, MovementState);   // Retrieve MovementState
  humanoid->texture = va_arg(args, SDL_Texture *);    // Retrieve SDL_Texture*

  // Initialize vector with variadic arguments
  vectorInit(&humanoid->bullets, 10, sizeof(Bullet), bulletConstructor,
             humanoid->animation, humanoid->movement,
             getResourcesInstance()->bulletTexture);

  humanoid->life = 100;
  humanoid->isAlive = true;

  va_end(args); // Clean up va_list
}

void humanoidCopyConstructor(Humanoid *destination, const Humanoid *source) {
  if (!destination || !source)
    return;

  destination->animation = source->animation;
  destination->movement = source->movement;
  destination->texture = source->texture;

  // Deep copy of the bullets Vector.
  destination->bullets = *vectorCopy(&source->bullets);

  destination->life = source->life;
  destination->isAlive = source->isAlive;
}

void humanoidDestructor(void *data) {
  if (!data)
    return;

  Humanoid *humanoid = (Humanoid *)data;

  // Use vectorFree to properly deallocate the Vector and its items.
  vectorFree(&humanoid->bullets, NULL);

  // Assuming that texture is managed elsewhere (shared resource), we don't
  // destroy it here.
}

void humanoidDie(Humanoid *humanoid) {
  if (!humanoid)
    return;

  humanoid->isAlive = false;
  animationStateHide(
      &humanoid
           ->animation); // Assuming animationStateHide is defined elsewhere.
}

void humanoidDecreaseLife(Humanoid *humanoid, unsigned int damage) {
  if (!humanoid || !humanoid->isAlive)
    return;

  if ((int)humanoid->life > (int)damage) {
    humanoid->life -= damage;
  } else {
    humanoid->life = 0;
    humanoidDie(humanoid);
  }
}
void humanoidShoot(Humanoid *humanoid) {
  Bullet *bullet = (Bullet *)vectorGet(&humanoid->bullets, 0);
  bullet->animation.isVisible = true;
  bullet->movement.position.x = humanoid->movement.position.x;
  bullet->movement.position.y = humanoid->movement.position.y;
}
