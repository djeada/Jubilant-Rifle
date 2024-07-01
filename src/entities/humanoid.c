#include "entities/humanoid.h"
#include "entities/bullet.h"
#include "entities/bullet_manager_factories.h"
#include "utils/consts.h"
#include "utils/resources.h"
#include <stdarg.h>

void humanoidConstructor(void *obj, ...) {
  va_list args;
  va_start(args, obj);

  humanoidConstructorGeneric(obj, args);

  va_end(args);
}

void humanoidConstructorGeneric(void *obj, va_list args) {
  if (!obj)
    return;

  Humanoid *humanoid = (Humanoid *)obj;
  humanoid->animation = va_arg(args, AnimationState);
  humanoid->movement = va_arg(args, MovementState);
  humanoid->texture = va_arg(args, SDL_Texture *);

  lifeConstructor(&humanoid->life, 100);
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
  lifeCopyConstructor(&destination->life, &source->life);
}

void humanoidDestructor(void *data) {
  if (!data)
    return;

  Humanoid *humanoid = (Humanoid *)data;
  bulletManagerDestructor(&humanoid->bulletManager);
}

bool humanoidIsAlive(Humanoid *humanoid) {
  return lifeIsAlive(&humanoid->life);
}

void humanoidDie(Humanoid *humanoid) {
  if (!humanoid)
    return;
  lifeDie(&humanoid->life);
  animationStateHide(&humanoid->animation);
}

void humanoidDecreaseLife(Humanoid *humanoid, unsigned int damage) {
  if (!humanoid || !humanoidIsAlive(humanoid))
    return;

  lifeTakeDamage(&humanoid->life, damage);
  if (!lifeIsAlive(&humanoid->life)) {
    humanoidDie(humanoid);
  }
}

void humanoidShoot(Humanoid *humanoid) {
  bulletManagerFireBullet(&humanoid->bulletManager, humanoid->movement.position,
                          humanoid->animation.isFacingLeft);
}
