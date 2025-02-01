#ifndef ENEMY_H
#define ENEMY_H

#include "entities/entity.h"
typedef struct {
    Entity base;
    float shootTimer;
} Enemy;

void enemy_update(Entity *self, float dt);

#endif
