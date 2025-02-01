#include "entities/enemy_array.h"

void enemy_array_init(EnemyArray *arr) {
    arr->capacity = 10;
    arr->count = 0;
    arr->data = malloc(sizeof(Enemy*) * arr->capacity);
}

void enemy_array_add(EnemyArray *arr, Enemy *enemy) {
    if (arr->count >= arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, sizeof(Enemy*) * arr->capacity);
    }
    arr->data[arr->count++] = enemy;
}

void enemy_array_update(EnemyArray *arr, float dt, BulletPool *pool) {
    for (int i = 0; i < arr->count; i++) {
        Enemy *enemy = arr->data[i];
        if (enemy->base.alive) {
            enemy->base.update((Entity*)enemy, dt);
            if (enemy->shootTimer <= 0) {
                bullet_pool_spawn(pool, enemy->base.pos.x + 20, enemy->base.pos.y + 50, 0, 150);
                enemy->shootTimer = 2.0f;
            } else {
                enemy->shootTimer -= dt;  // Ensure shootTimer decreases over time
            }
        }
    }
}

void enemy_array_destroy(EnemyArray *arr) {
    for (int i = 0; i < arr->count; i++) {
        if (arr->data[i])
            entity_destroy((Entity*)arr->data[i]);
    }
    free(arr->data);
}
