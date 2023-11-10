#include "entities/humanoid_factories.h"
#include "utils/resources.h"

void createHumanoid(Humanoid *newHumanoid, int initialSpriteIndex,
                    bool isFacingLeft, bool isWalking, int initialPosX,
                    int initialPosY, int initialVelX, int initialVelY,
                    bool isVisible, SDL_Texture *texture) {

  AnimationState animation;
  animationStateConstructor(&animation, initialSpriteIndex, isFacingLeft,
                            isWalking, isVisible);

  MovementState movement;
  movementStateConstructor(&movement, initialPosX, initialPosY, initialVelX,
                           initialVelY);

  humanoidConstructor(newHumanoid, animation, movement, texture);
}

void createPlayerHumanoid(Humanoid *newHumanoid) {
  int initialSpriteIndex = 0; // Specific to player
  bool isFacingLeft = false;
  bool isWalking = false;
  int initialPosX = 100; // Starting position for player
  int initialPosY = 100;
  int initialVelX = 0;
  int initialVelY = 0;
  bool isVisible = true;

  createHumanoid(newHumanoid, initialSpriteIndex, isFacingLeft, isWalking,
                 initialPosX, initialPosY, initialVelX, initialVelY, isVisible,
                 getResourcesInstance()->playerTexture);
}
