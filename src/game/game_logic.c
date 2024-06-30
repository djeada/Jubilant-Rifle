#include "game/game_logic.h"
#include "entities/bullet.h"
#include "entities/enemy.h"
#include "utils/consts.h"
#include "utils/resources.h"
#include "utils/time_manager.h"

void updateAnimation(Humanoid *humanoid) {
  if (movementStateIsMoving(&humanoid->movement)) {
    if (timeManagerGetTime() % 3 == 0) {
      animationStateIncrementSprite(&humanoid->animation);
    }
  } else {
    animationStateStop(&humanoid->animation);
  }
}

void updatePosition(Humanoid *humanoid) {
  movementStateMoveHorizontal(&humanoid->movement);
}

bool checkCollisionWithBullet(Humanoid *humanoid, const Bullet *bullet) {
  int leftB = bullet->movement.position.x;
  int rightB = leftB + BULLET_WIDTH;
  int topB = bullet->movement.position.y;
  int bottomB = topB + BULLET_HEIGHT;
  int leftE = humanoid->movement.position.x;
  int rightE = leftE + HUMANOID_FRAME_WIDTH;
  int topE = humanoid->movement.position.y;
  int bottomE = topE + HUMANOID_FRAME_HEIGHT;

  return !(bottomB <= topE || topB >= bottomE || rightB <= leftE ||
           leftB >= rightE);
}

void handleShooting(Humanoid *player, Vector *enemies) {
  for (size_t i = 0; i < player->bulletManager.bullets.size; ++i) {
    Bullet *bullet = (Bullet *)player->bulletManager.bullets.items[i];

    if (movementStateIsMoving(&bullet->movement)) {
      movementStateMoveHorizontal(&bullet->movement);

      for (size_t j = 0; j < enemies->size; ++j) {
        Enemy *enemy = (Enemy *)enemies->items[j];
        if (checkCollisionWithBullet(&enemy->base, bullet)) {
          humanoidDecreaseLife(&enemy->base, 1000);
          if (!enemy->base.isAlive) {
            humanoidDie(&enemy->base);
          }
          movementStateStop(&bullet->movement);
          animationStateHide(&bullet->animation);
          break;
        }
      }

      if (pointDistance(&player->movement.position,
                        &bullet->movement.position) > 500) {
        movementStateStop(&bullet->movement);
        animationStateHide(&bullet->animation);
      }
    }
  }
}

bool handleCollisions(Humanoid *player, const Map *map) {
  for (size_t i = 0; i < map->platformCount; ++i) {
    if (checkCollision(player, &map->platforms[i])) {
      player->movement.position.y =
          map->platforms[i].y - 0.95 * HUMANOID_FRAME_HEIGHT;
      return true;
    }
  }
  return false;
}

void applyGravity(Humanoid *player, bool onPlatform) {
  if (onPlatform) {
    movementStateMoveJump(&player->movement);
  } else if (!movementStateIsJumping(&player->movement)) {
    movementStateFall(&player->movement);
  }
}

void checkWorldBounds(Humanoid *player, Map *map) {
  if (player->movement.position.x < 0) {
    player->movement.position.x = 0;
  }
  if (player->movement.position.x > map->width) {
    player->movement.position.x = WORLD_WIDTH;
  }
  if (player->movement.position.y < 0) {
    player->movement.position.y = 0;
  }
  if (player->movement.position.y > WORLD_HEIGHT) {
    humanoidDie(player);
  }
}

void updatePlayerState(Humanoid *player, Vector *enemies, Map *map) {
  updateAnimation(player);
  updatePosition(player);
  handleShooting(player, enemies);
  checkWorldBounds(player, map);

  bool onPlatform = handleCollisions(player, map);
  applyGravity(player, onPlatform);

  movementStateStop(&player->movement);
}

bool checkCollision(const Humanoid *humanoid, const Platform *platform) {
  int leftA = humanoid->movement.position.x;
  int rightA = leftA + HUMANOID_FRAME_WIDTH / 2;
  int topA = humanoid->movement.position.y;
  int bottomA = topA + HUMANOID_FRAME_HEIGHT;
  int leftB = platform->x;
  int rightB = leftB + platform->width;
  int topB = platform->y;
  int bottomB = topB + platform->height;

  return !(bottomA <= topB || topA >= bottomB || rightA <= leftB ||
           leftA >= rightB);
}

void updateEnemies(Vector *enemies) {
  for (size_t i = 0; i < enemies->size; ++i) {
    Enemy *enemy = (Enemy *)enemies->items[i];

    updatePosition(&enemy->base);
    updateAnimation(&enemy->base);

    if ((enemy->base.animation.isFacingLeft &&
         enemy->base.movement.position.x < enemy->patrolStart.x) ||
        (!enemy->base.animation.isFacingLeft &&
         enemy->base.movement.position.x > enemy->patrolEnd.x) ||
        enemy->base.movement.velocity.x == 0) {
      enemy->base.animation.isFacingLeft = !enemy->base.animation.isFacingLeft;
      enemy->base.movement.velocity.x =
          enemy->base.animation.isFacingLeft ? -1 : 1;
    }
  }
}
