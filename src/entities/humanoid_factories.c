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

void createPlayerHumanoid(Humanoid *newHumanoid) {
  Point initialPos = {100, 100};
  Point initialVel = {0, 0};
  setupHumanoid(newHumanoid, 0, false, initialPos, initialVel, true,
                getResourcesInstance()->playerTexture);
}

void createEnemyHumanoid(Humanoid *newHumanoid) {
  Point initialPos = {100, 100};
  Point initialVel = {0, 0};
  setupHumanoid(newHumanoid, 0, false, initialPos, initialVel, true,
                getResourcesInstance()->enemyTexture);
}

void createHumanoids(Vector *newHumanoids, Map *map,
                     void (*createHumanoidFunc)(void *, va_list)) {
  vectorInit(newHumanoids, map->platformCount, sizeof(Humanoid),
             createHumanoidFunc);
  for (size_t i = 0; i < map->platformCount; ++i) {
    Humanoid *humanoid = (Humanoid *)newHumanoids->items[i];
    Point position = {map->platforms[i].x + map->platforms[i].width / 2,
                      map->platforms[i].y - PLAYER_FRAME_HEIGHT};
    pointCopyConstructor(&humanoid->movement.position, &position);
  }
}

void createPlayerHumanoidGeneric(void *obj, va_list args) {
  (void)args; // Unused
  createPlayerHumanoid((Humanoid *)obj);
}

void createEnemyHumanoidGeneric(void *obj, va_list args) {
  (void)args; // Unused
  createEnemyHumanoid((Humanoid *)obj);
}

void createEnemies(Vector *newEnemies, Map *map) {
  createHumanoids(newEnemies, map, createEnemyHumanoidGeneric);
}
