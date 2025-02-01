#ifndef ENEMY_ARRAY_H
#define ENEMY_ARRAY_H

#include <stdlib.h>
#include "entities/entity.h"
#include "entities/bullet_pool.h"
#include "entities/enemy.h"

typedef struct {
    Enemy **data;
    int count;
    int capacity;
} EnemyArray;

void enemyArrayInit(EnemyArray *arr);
void enemyArrayAdd(EnemyArray *arr, Enemy *enemy);
void enemyArrayUpdate(EnemyArray *arr, float dt, BulletPool *pool);
void enemyArrayDestroy(EnemyArray *arr);

#endif // ENEMY_ARRAY_H
