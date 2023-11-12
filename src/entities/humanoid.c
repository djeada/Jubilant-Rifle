#include "entities/humanoid.h"
#include "entities/bullet.h"
#include "entities/bullet_manager_factories.h"
#include "utils/consts.h"
#include "utils/resources.h"
#include <stdarg.h>

void humanoidConstructor(void *obj, ...) {
  va_list args;
  va_start(args, obj); // Initialize va_list with the last known fixed argument

  humanoidConstructorGeneric(
      obj, args); // Pass the va_list to the actual constructor

  va_end(args); // Clean up va_list
}

void humanoidConstructorGeneric(void *obj, va_list args) {
  if (!obj)
    return;

  Humanoid *humanoid = (Humanoid *)obj;
  humanoid->animation =
      va_arg(args, AnimationState); // Retrieve AnimationState from Generic args
  humanoid->movement =
      va_arg(args, MovementState); // Retrieve MovementState from Generic args
  humanoid->texture =
      va_arg(args, SDL_Texture *); // Retrieve SDL_Texture* from Generic args

  humanoid->life = 100;
  humanoid->isAlive = true;

  createBulletManagerForHumanoid(&humanoid->bulletManager, humanoid);
}

void humanoidCopyConstructor(Humanoid *destination, const Humanoid *source) {
  if (!destination || !source)
    return;

  destination->animation = source->animation;
  destination->movement = source->movement;
  destination->texture = source->texture;

  bulletManagerCopyConstructor(&destination->bulletManager,
                               &source->bulletManager);

  destination->life = source->life;
  destination->isAlive = source->isAlive;
}

void humanoidDestructor(void *data) {
  if (!data)
    return;

  Humanoid *humanoid = (Humanoid *)data;

  // Use vectorFree to properly deallocate the Vector and its items.
  bulletManagerDestructor(&humanoid->bulletManager);

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
  bulletManagerFireBullet(&humanoid->bulletManager, humanoid->movement.position,
                          humanoid->animation.isFacingLeft);
}
