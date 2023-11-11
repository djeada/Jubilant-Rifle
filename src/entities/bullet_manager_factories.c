#include "entities/humanoid_factories.h"
#include "utils/consts.h"
#include "utils/resources.h"

void createBulletManagerForHumanoid(BulletManager *bulletManager,
                                    Humanoid *humanoid) {
  AnimationState animation;
  animationStateConstructor(&animation, 0, humanoid->animation.isFacingLeft,
                            false);

  MovementState movement;
  movementStateConstructor(&movement, humanoid->movement.position.x,
                           humanoid->movement.position.y, 0, 0);

  bulletManagerConstructor(bulletManager, NUM_BULLETS, animation, movement,
                           getResourcesInstance()->bulletTexture);
}
