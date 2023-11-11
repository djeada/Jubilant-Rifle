#ifndef BULLET_H
#define BULLET_H

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
} Bullet;

// Function declarations
void bulletConstructor(void *obj, ...);
void bulletConstructorVariadic(void *obj, va_list args);
void bulletCopy(Bullet *destination, const Bullet *source);
void bulletMove(Bullet *bullet);

#endif
