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

void bulletConstructor(void *obj, ...);
void bulletConstructorGeneric(void *obj, va_list args);
void bulletCopyConstructor(Bullet *destination, const Bullet *source);
void bulletCopyConstructorGeneric(void *destination, void *source);
void bulletMove(Bullet *bullet);

#endif
