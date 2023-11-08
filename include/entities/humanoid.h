#ifndef HUMANOID_H
#define HUMANOID_H

#include "animation/animation_state.h"
#include "bullet.h"
#include "entities/movement_state.h"
#include "utils/vector.h"
#include <SDL2/SDL_image.h>
#include <stdbool.h>

typedef struct {
  AnimationState animation;
  MovementState movement;
  SDL_Texture *texture;
  Vector bullets;
  short life;
  bool isAlive;
} Humanoid;

void humanoidConstructor(Humanoid *humanoid, int spriteIndex, bool facingLeft,
                         bool walking, int posX, int posY, int startX, int endX,
                         SDL_Texture *tex, short initialLife, bool alive,
                         bool visible);
void humanoidDefaultConstructor(Humanoid *humanoid);
void humanoidCopyConstructor(Humanoid *destination, const Humanoid *source);
void humanoidDestructor(void *data);
void humanoidDie(Humanoid *humanoid);
void humanoidDecreaseLife(Humanoid *humanoid, unsigned int damage);

#endif
