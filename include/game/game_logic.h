
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include "entities/humanoid.h"
#include "map/map.h"
#include "utils/vector.h"

#ifdef __cplusplus
extern "C" {
#endif

void updateHumanoid(Humanoid *humanoid, Map *map);
bool checkCollision(const Humanoid *humanoid, const Platform *platform);

#ifdef __cplusplus
}
#endif

#endif // GAME_LOGIC_H
