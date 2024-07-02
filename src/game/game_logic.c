#include "game/game_logic.h"
#include "entities/bullet.h"
#include "entities/enemy.h"
#include "utils/consts.h"
#include "utils/resources.h"
#include "utils/time_manager.h"

void incrementAnimation(Humanoid *humanoid) {
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

void fillApproximateEnemies(Vector *enemies, Player *player,
                            Vector *approxEnemies, float proximityThreshold) {
  approxEnemies->size = 0;

  for (size_t i = 0; i < enemies->size; ++i) {
    Enemy *enemy = (Enemy *)enemies->items[i];
    if (enemyIsAlive(enemy)) {
      if (pointDistance(&player->base.movement.position,
                        &enemy->base.movement.position) <= proximityThreshold) {
        if (approxEnemies->size >= approxEnemies->capacity) {
          approxEnemies->capacity *= 2;
          approxEnemies->items = realloc(
              approxEnemies->items, approxEnemies->capacity * sizeof(void *));
        }
        approxEnemies->items[approxEnemies->size++] = enemy;
      }
    }
  }
}

void handlePlayerShooting(Player *player, Vector *enemies) {
  Vector approxEnemies;
  approxEnemies.size = 0;
  approxEnemies.capacity = 10;
  approxEnemies.items = malloc(approxEnemies.capacity * sizeof(void *));
  float proximityThreshold = 1000.0f;
  fillApproximateEnemies(enemies, player, &approxEnemies, proximityThreshold);

  for (size_t i = 0; i < player->base.bulletManager.bullets.size; ++i) {
    Bullet *bullet = (Bullet *)player->base.bulletManager.bullets.items[i];

    if (movementStateIsMoving(&bullet->movement)) {
      movementStateMoveHorizontal(&bullet->movement);

      for (size_t j = 0; j < approxEnemies.size; ++j) {
        Enemy *enemy = (Enemy *)approxEnemies.items[j];
        if (checkCollisionWithBullet(&enemy->base, bullet)) {
          humanoidDecreaseLife(&enemy->base, 1000);
          if (!enemyIsAlive(enemy)) {
            humanoidDie(&enemy->base);
            playerIncreaseExperience(player, 1000);
          }
          movementStateStop(&bullet->movement);
          animationStateHide(&bullet->animation);
          break;
        }
      }

      if (pointDistance(&player->base.movement.position,
                        &bullet->movement.position) > 500) {
        movementStateStop(&bullet->movement);
        animationStateHide(&bullet->animation);
      }
    }
  }

  free(approxEnemies.items);
}

void handleEnemyShooting(Enemy *enemy, Player *player) {
  for (size_t i = 0; i < enemy->base.bulletManager.bullets.size; ++i) {
    Bullet *bullet = (Bullet *)enemy->base.bulletManager.bullets.items[i];

    if (movementStateIsMoving(&bullet->movement)) {
      movementStateMoveHorizontal(&bullet->movement);

      if (checkCollisionWithBullet(&player->base, bullet)) {
        // humanoidDecreaseLife(&player->base, 1000);
        if (!playerIsAlive(player)) {
          humanoidDie(&player->base);
        }
        movementStateStop(&bullet->movement);
        animationStateHide(&bullet->animation);
        break;
      }

      if (pointDistance(&enemy->base.movement.position,
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
    player->movement.position.x = map->width;
  }
  if (player->movement.position.y < 0) {
    player->movement.position.y = 0;
  }
  if (player->movement.position.y > map->height) {
    humanoidDie(player);
  }
}

void updatePlayerState(Player *player, Vector *enemies, Map *map) {
  if (!playerIsAlive(player))
    return;

  incrementAnimation(&player->base);
  updatePosition(&player->base);
  handlePlayerShooting(player, enemies);
  checkWorldBounds(&player->base, map);

  bool onPlatform = handleCollisions(&player->base, map);
  applyGravity(&player->base, onPlatform);

  movementStateStop(&player->base.movement);
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

void updateEnemies(Vector *enemies, Player *player) {
  for (size_t i = 0; i < enemies->size; ++i) {
    Enemy *enemy = (Enemy *)enemies->items[i];

    if (!enemyIsAlive(enemy)) {
      continue;
    }

    if (pointDistanceX(&enemy->base.movement.position,
                       &player->base.movement.position) < 500 &&
        pointDistanceY(&enemy->base.movement.position,
                       &player->base.movement.position) < 100 &&
        playerIsAlive(player)) {

      enemy->base.animation.isFacingLeft = isPointLeftOf(
          &player->base.movement.position, &enemy->base.movement.position);
      animationStateStop(&enemy->base.animation);
      humanoidShoot(&enemy->base);
      handleEnemyShooting(enemy, player);
    }

    else {

      updatePosition(&enemy->base);
      incrementAnimation(&enemy->base);

      bool isFacingLeft = enemy->base.animation.isFacingLeft;
      float posX = enemy->base.movement.position.x;
      float startX = enemy->patrolStart.x;
      float endX = enemy->patrolEnd.x;

      bool isBeyondLeftBound = isFacingLeft && posX < startX;
      bool isBeyondRightBound =
          !isFacingLeft && posX + HUMANOID_FRAME_WIDTH > endX;

      if (isBeyondLeftBound || isBeyondRightBound) {
        enemy->base.animation.isFacingLeft = !isFacingLeft;
        enemy->base.movement.velocity.x =
            !isFacingLeft ? -ENEMY_MOVE_SPEED : ENEMY_MOVE_SPEED;
      }
    }
  }
}
