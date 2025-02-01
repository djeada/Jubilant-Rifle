#ifndef ENEMY_H
#define ENEMY_H

#include "entities/entity.h"
typedef struct {
    Entity base;
    float shootTimer;
} Enemy;
Enemy* enemy_create(float x, float y) ;
void enemy_update(Entity *self, float dt);

#endif
