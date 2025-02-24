#ifndef PHYSICS_H
#define PHYSICS_H

#include "SDL2/SDL.h"
#include "entities/bullet_pool.h"
#include "entities/enemy_array.h"
#include "entities/player.h"

void handleCollisions(BulletPool *pool, Player *player, EnemyArray *enemies);

#endif // PHYSICS_H

