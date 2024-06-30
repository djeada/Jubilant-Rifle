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
  bulletManagerDestructor(&humanoid->bulletManager);
}

void humanoidDie(Humanoid *humanoid) {
  if (!humanoid)
    return;

  humanoid->isAlive = false;
  animationStateHide(&humanoid->animation);
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
