#include "entities/humanoid_factories.h"
#include "utils/consts.h"
#include "utils/resources.h"
#include <stdlib.h>
#include <time.h>

void setupHumanoid(Humanoid *humanoid, int initialSpriteIndex,
                   bool isFacingLeft, Point initialPos, Point initialVel,
                   bool isVisible, SDL_Texture *texture) {
  AnimationState animation;
  animationStateConstructor(&animation, initialSpriteIndex, isFacingLeft,
                            isVisible);

  MovementState movement;
  movementStateConstructor(&movement, initialPos, initialVel);

  humanoidConstructor(humanoid, animation, movement, texture);
}

void createEnemyHumanoid(Enemy *newHumanoid) {
  Point initialPos = {100, 100};
  int randomDirection =
      (rand() % 2 == 0) ? -ENEMY_MOVE_SPEED : ENEMY_MOVE_SPEED;
  Point initialVel = {randomDirection, 0};
  setupHumanoid(&newHumanoid->base, 0, randomDirection < 0, initialPos,
                initialVel, true, getResourcesInstance()->enemyTexture);
}

void createEnemyHumanoidGeneric(void *obj, va_list args) {
  (void)args;
  createEnemyHumanoid((Enemy *)obj);
}

void createEnemies(Vector *newEnemies, Map *map) {
  vectorInit(newEnemies, map->platformCount, sizeof(Enemy),
             createEnemyHumanoidGeneric);
  for (size_t i = 0; i < map->platformCount; ++i) {
    Enemy *humanoid = (Enemy *)newEnemies->items[i];
    Point position = getRandomPositionOnPlatform(&map->platforms[i]);
    pointCopyConstructor(&humanoid->base.movement.position, &position);
    humanoid->patrolStart = makePoint(map->platforms[i].x, map->platforms[i].y);
    humanoid->patrolEnd = makePoint(
        map->platforms[i].x + map->platforms[i].width, map->platforms[i].y);
  }
}
