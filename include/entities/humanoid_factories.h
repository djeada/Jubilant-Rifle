#ifndef HUMANOID_FACTORIES_H
#define HUMANOID_FACTORIES_H

#include "entities/humanoid.h"

void createHumanoid(Humanoid *newHumanoid, int initialSpriteIndex,
                    bool isFacingLeft, bool isWalking, int initialPosX,
                    int initialPosY, int initialVelX, int initialVelY,
                    bool isVisible, SDL_Texture *texture);
void createPlayerHumanoid(Humanoid *newHumanoid);

#endif // HUMANOID_FACTORIES_H
