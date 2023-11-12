#ifndef HUMANOID_FACTORIES_H
#define HUMANOID_FACTORIES_H

#include "entities/humanoid.h"

void createHumanoid(Humanoid *newHumanoid, int initialSpriteIndex,
                    bool isFacingLeft, Point initialPos, Point initialVel,
                    bool isVisible, SDL_Texture *texture);
void createPlayerHumanoid(Humanoid *newHumanoid);
void createPlayerHumanoidVariadic(void *obj, va_list args);

#endif // HUMANOID_FACTORIES_H
