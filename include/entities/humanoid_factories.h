#ifndef HUMANOID_FACTORIES_H
#define HUMANOID_FACTORIES_H

#include "entities/enemy.h"
#include "entities/humanoid.h"
#include "map/map.h"

void setupHumanoid(Humanoid *humanoid, int initialSpriteIndex,
                   bool isFacingLeft, Point initialPos, Point initialVel,
                   bool isVisible, SDL_Texture *texture);
void createHumanoid(Humanoid *newHumanoid, int initialSpriteIndex,
                    bool isFacingLeft, Point initialPos, Point initialVel,
                    bool isVisible, SDL_Texture *texture);
void createEnemyHumanoid(Enemy *newHumanoid);
void createEnemyHumanoidGeneric(void *obj, va_list args);
void createEnemies(Vector *newEnemies, Map *map);

#endif // HUMANOID_FACTORIES_H
