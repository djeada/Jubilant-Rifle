#ifndef HUMANOID_H
#define HUMANOID_H

#include "entities/bullet_manager.h"
#include "entities/life.h"

typedef struct {
  AnimationState animation;
  MovementState movement;
  SDL_Texture *texture;
  BulletManager bulletManager;
  Life life; 
} Humanoid;

void humanoidConstructor(void *obj, ...);
void humanoidConstructorGeneric(void *obj, va_list args);
void humanoidCopyConstructor(Humanoid *destination, const Humanoid *source);
void humanoidDestructor(void *data);
bool humanoidIsAlive(Humanoid *humanoid);
void humanoidDie(Humanoid *humanoid);
void humanoidDecreaseLife(Humanoid *humanoid, unsigned int damage);
void humanoidShoot(Humanoid *humanoid);

#endif
