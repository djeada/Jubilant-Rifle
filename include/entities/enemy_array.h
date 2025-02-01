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

void enemy_array_init(EnemyArray *arr);
void enemy_array_add(EnemyArray *arr, Enemy *enemy);
void enemy_array_update(EnemyArray *arr, float dt, BulletPool *pool);
void enemy_array_destroy(EnemyArray *arr);

#endif // ENEMY_ARRAY_H
