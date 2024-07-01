
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "entities/player.h"
#include "map/map.h"
#include "utils/vector.h"

bool checkCollision(const Humanoid *humanoid, const Platform *platform);
void updatePlayerState(Player *humanoid, Vector *enemies, Map *map);
void updateEnemies(Vector *enemies);

#endif // GAME_LOGIC_H
