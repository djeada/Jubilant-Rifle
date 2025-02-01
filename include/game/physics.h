#ifndef PHYSICS_H
#define PHYSICS_H

#include "SDL2/SDL.h"   
#include "entities/entity.h" 
#include "entities/bullet_pool.h" 
#include "entities/enemy_array.h" 


void handleCollisions(BulletPool *pool, Entity *player, EnemyArray *enemies);

#endif // PHYSICS_H
