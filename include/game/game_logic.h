
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "entities/humanoid.h"
#include "map/map.h"
#include "utils/vector.h"

bool checkCollision(const Humanoid *humanoid, const Platform *platform);
void updateHumanoid(Humanoid *humanoid, Map *map);
void updateEnemies(Vector *enemies, Map *map);

#endif // GAME_LOGIC_H
