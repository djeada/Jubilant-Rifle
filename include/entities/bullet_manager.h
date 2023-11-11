#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include "animation/animation_state.h"
#include "entities/movement_state.h"
#include "utils/point.h"
#include "utils/vector.h"
#include <SDL2/SDL_image.h>
#include <stdbool.h>

typedef struct {
  Vector bullets;       // Vector to store bullets
  size_t nextAvailable; // Index of the next available bullet
} BulletManager;

void bulletManagerConstructor(BulletManager *manager, int bulletNumber,
                              AnimationState animation, MovementState movement,
                              SDL_Texture *texture);
void bulletManagerDestructor(BulletManager *manager);
void bulletManagerFireBullet(BulletManager *manager, Point position,
                             bool isFacingLeft);

#endif
