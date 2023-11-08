// game_logic.c

#include "game/game_logic.h"
#include "utils/resources.h"
#include "utils/time_manager.h"

void updateHumanoid(Humanoid *humanoid, Map *map) {
  if (movementStateIsMoving(&humanoid->movement) &&
      timeManagerGetTime() % 3 == 0) {
    animationStateIncrementSprite(&humanoid->animation);
  }

  // Update humanoid position
  movementStateMoveHorizontal(&humanoid->movement);

  bool onPlatform = false;

  // Collision detection with the ground and platforms
  for (size_t i = 0; i < map->platformCount; i++) {
    if (checkCollision(humanoid, &map->platforms[i])) {
      // Align the humanoid's position to the top of the platform
      humanoid->movement.position.y =
          map->platforms[i].y -
          95; // Subtract the frame height to place the humanoid on top
      onPlatform = true;
      break; // Break out of the loop since we've found a platform
    }
  }

  // Apply gravity if not on any platform

  if (onPlatform) {
    movementStateMoveJump(&humanoid->movement);
  } else if (!movementStateIsJumping(&humanoid->movement)) {
    movementStateFall(&humanoid->movement);
  }
  movementStateStop(&humanoid->movement);
}

bool checkCollision(const Humanoid *humanoid, const Platform *platform) {
  // Basic AABB collision detection
  int leftA = humanoid->movement.position.x;
  int rightA = leftA + 20; // frameWidth/2
  int topA = humanoid->movement.position.y;
  int bottomA = topA + 100; // humanoid height (adjusted if necessary)

  int leftB = platform->x;
  int rightB = leftB + platform->width;
  int topB = platform->y;
  int bottomB = topB + platform->height;

  // Conditions for no collision
  if (bottomA <= topB) {
    return false;
  }
  if (topA >= bottomB) {
    return false;
  }
  if (rightA <= leftB) {
    return false;
  }
  if (leftA >= rightB) {
    return false;
  }

  // Additional condition to check if the humanoid comes from above
  if (bottomA > topB) {
    return true; // The bottom of the humanoid was above the top of the platform
                 // in the last frame
  }

  return false; // The humanoid is not coming from above the platform
}
