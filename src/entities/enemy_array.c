#include "entities/enemy_array.h"
#include "entities/entity.h"
#include "entities/bullet_pool.h"
#include <stdlib.h>

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

/*
 * Update each enemy and remove dead ones from the array.
 * This prevents dead enemies from updating or spawning bullets.
 */
void enemy_array_update(EnemyArray *arr, float dt, BulletPool *pool) {
    for (int i = 0; i < arr->count; ) {
        Enemy *enemy = arr->data[i];
        
        // Mark enemy as dead if health is depleted.
        if (enemy->base.health <= 0)
            enemy->base.alive = 0;
        
        if (!enemy->base.alive) {
            // Remove enemy by replacing it with the last one.
            arr->data[i] = arr->data[arr->count - 1];
            arr->count--;
            continue;  // Process the swapped enemy at index i.
        }
        
        // Update enemy logic.
        enemy->base.update((Entity*)enemy, dt);
        
        // Process shooting.
        enemy->shootTimer -= dt;
        if (enemy->shootTimer <= 0) {
            bullet_pool_spawn(pool,
                              enemy->base.pos.x + 20,
                              enemy->base.pos.y + 50,
                              0,
                              150);
            enemy->shootTimer = 2.0f;
        }
        i++;
    }
}

void enemy_array_destroy(EnemyArray *arr) {
    for (int i = 0; i < arr->count; i++) {
        if (arr->data[i])
            entity_destroy((Entity*)arr->data[i]);
    }
    free(arr->data);
}
