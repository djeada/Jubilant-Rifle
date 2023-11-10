#ifndef HUMANOID_H
#define HUMANOID_H

#include "bullet.h"
#include "utils/vector.h"

typedef struct {
  AnimationState animation;
  MovementState movement;
  SDL_Texture *texture;
  Vector bullets;
  short life;
  bool isAlive;
} Humanoid;

void humanoidConstructor(void *obj, ...);
void humanoidCopyConstructor(Humanoid *destination, const Humanoid *source);
void humanoidDestructor(void *data);
void humanoidDie(Humanoid *humanoid);
void humanoidDecreaseLife(Humanoid *humanoid, unsigned int damage);
void humanoidShoot(Humanoid *humanoid);

#endif
