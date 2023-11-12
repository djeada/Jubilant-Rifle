#include "entities/humanoid_factories.h"
#include "utils/resources.h"

void createHumanoid(Humanoid *newHumanoid, int initialSpriteIndex,
                    bool isFacingLeft, Point initialPos, Point initialVel,
                    bool isVisible, SDL_Texture *texture) {

  AnimationState animation;
  animationStateConstructor(&animation, initialSpriteIndex, isFacingLeft,
                            isVisible);

  MovementState movement;
  movementStateConstructor(&movement, initialPos, initialVel);

  humanoidConstructor(newHumanoid, animation, movement, texture);
}

void createPlayerHumanoid(Humanoid *newHumanoid) {
  int initialSpriteIndex = 0;
  bool isFacingLeft = false;
  Point initialPos = {100, 100};
  Point initialVel = {0, 0};
  bool isVisible = true;

  createHumanoid(newHumanoid, initialSpriteIndex, isFacingLeft, initialPos,
                 initialVel, isVisible, getResourcesInstance()->playerTexture);
}
void createPlayerHumanoidVariadic(void *obj, va_list args) {
  (void)args; // Cast args to void to indicate it's unused
  // Assuming obj is a Humanoid object.
  // If not, you'll need to adjust this part accordingly.
  Humanoid *newHumanoid = (Humanoid *)obj;

  // Process other arguments with va_arg if needed
  // ...

  // Call the createPlayerHumanoid function
  createPlayerHumanoid(newHumanoid);
}