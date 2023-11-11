#include "entities/humanoid_factories.h"
#include "utils/resources.h"

void createBulletManagerForHumanoid(BulletManager *bulletManager,
                                    Humanoid *humanoid) {
  AnimationState animation;
  animationStateConstructor(&animation, 0, humanoid->animation.isFacingLeft,
                            true, false);

  MovementState movement;
  movementStateConstructor(&movement, humanoid->movement.position.x,
                           humanoid->movement.position.y, 0, 0);

  bulletManagerConstructor(bulletManager, animation, movement,
                           getResourcesInstance()->bulletTexture);
}
