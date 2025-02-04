#ifndef ENEMY_H
#define ENEMY_H

#include "entities/entity.h"

typedef struct {
    Entity base;
    float shootTimer;
} Enemy;

Enemy* enemyCreate(float x, float y) ;
void enemyUpdate(Entity *self, float dt);
bool isEnemyAlive(const Enemy *enemy);
Direction getEnemyFacingDirection(const Enemy *enemy);
bool isEnemyMoving(const Enemy *enemy);

#endif
