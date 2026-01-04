#ifndef PHYSICS_H
#define PHYSICS_H

#include "SDL2/SDL.h"
#include "entities/bullet_pool.h"
#include "entities/enemy_array.h"
#include "entities/player.h"
#include "map/map.h"

void handleCollisions(BulletPool *pool, Player *player, EnemyArray *enemies);
void applyPhysics(Player *player, EnemyArray *enemies, const Map *map, float dt);

#endif // PHYSICS_H
