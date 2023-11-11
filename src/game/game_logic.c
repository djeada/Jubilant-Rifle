// game_logic.c

#include "game/game_logic.h"
#include "entities/bullet.h"
#include "utils/resources.h"
#include "utils/time_manager.h"

void updateAnimation(Humanoid *player) {
  if (movementStateIsMoving(&player->movement) &&
      timeManagerGetTime() % 3 == 0) {
    animationStateIncrementSprite(&player->animation);
  }
}

void updatePosition(Humanoid *player) {
  movementStateMoveHorizontal(&player->movement);
}

void handleShooting(Humanoid *player) {
  for (size_t i = 0; i < player->bulletManager.bullets.size; i++) {
    Bullet *bullet = (Bullet *)player->bulletManager.bullets.items[i];
    movementStateMoveHorizontal(&bullet->movement);
  }
}

bool handleCollisions(Humanoid *player, Map *map) {
  bool onPlatform = false;
  for (size_t i = 0; i < map->platformCount; i++) {
    if (checkCollision(player, &map->platforms[i])) {
      player->movement.position.y = map->platforms[i].y - 95;
      onPlatform = true;
      break;
    }
  }
  return onPlatform;
}
void applyGravity(Humanoid *player, bool onPlatform) {
  if (!onPlatform) {
    if (!movementStateIsJumping(&player->movement)) {
      movementStateFall(&player->movement);
    }
  } else {
    movementStateMoveJump(&player->movement);
  }
}

void checkWorldBounds(Humanoid *player) {
  const int worldWidth = 3000;
  const int worldHeight = 1200;
  // Check left boundary
  if (player->movement.position.x < 0) {
    player->movement.position.x = 0;
  }

  // Check right boundary
  if (player->movement.position.x > worldWidth) {
    player->movement.position.x = worldWidth;
  }

  // Check top boundary
  if (player->movement.position.y < 0) {
    player->movement.position.y = 0;
  }

  // Check bottom boundary (death condition)
  if (player->movement.position.y > worldHeight) {
    // Handle death or failure
    humanoidDie(player);
  }
}

void updateHumanoid(Humanoid *player, Map *map) {
  updateAnimation(player);
  updatePosition(player);
  handleShooting(player);
  checkWorldBounds(player);

  bool onPlatform = handleCollisions(player, map);
  applyGravity(player, onPlatform);

  movementStateStop(&player->movement);
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
