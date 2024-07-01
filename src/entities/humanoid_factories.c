#include "entities/humanoid_factories.h"
#include "utils/consts.h"
#include "utils/resources.h"

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

// TODO: Position Map dependent
void createPlayerHumanoid(Humanoid *newHumanoid) {
  Point initialPos = {100, 100};
  Point initialVel = {0, 0};
  setupHumanoid(newHumanoid, 0, false, initialPos, initialVel, true,
                getResourcesInstance()->playerTexture);
}

// TODO: Position Platform dependent
void createEnemyHumanoid(Enemy *newHumanoid) {
  Point initialPos = {100, 100};
  Point initialVel = {0, 0};
  setupHumanoid(&newHumanoid->base, 0, false, initialPos, initialVel, true,
                getResourcesInstance()->enemyTexture);
}

void createHumanoids(Vector *newHumanoids, Map *map,
                     void (*createHumanoidFunc)(void *, va_list)) {
  vectorInit(newHumanoids, map->platformCount, sizeof(Enemy),
             createHumanoidFunc);
  for (size_t i = 0; i < map->platformCount; ++i) {
    Enemy *humanoid = (Enemy *)newHumanoids->items[i];
    Point position = {map->platforms[i].x + map->platforms[i].width / 2,
                      map->platforms[i].y - HUMANOID_FRAME_HEIGHT};
    pointCopyConstructor(&humanoid->base.movement.position, &position);
    humanoid->patrolStart = makePoint(map->platforms[i].x, map->platforms[i].y);
    humanoid->patrolEnd = makePoint(
        map->platforms[i].x + map->platforms[i].width, map->platforms[i].y);
  }
}

void createPlayerHumanoidGeneric(void *obj, va_list args) {
  (void)args;
  createPlayerHumanoid((Humanoid *)obj);
}

void createEnemyHumanoidGeneric(void *obj, va_list args) {
  (void)args;
  createEnemyHumanoid((Enemy *)obj);
}

void createEnemies(Vector *newEnemies, Map *map) {
  createHumanoids(newEnemies, map, createEnemyHumanoidGeneric);
}
