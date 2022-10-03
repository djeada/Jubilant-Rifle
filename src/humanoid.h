#ifndef HUMANOID_H
#define HUMANOID_H

#include "bullet.h"
#include <SDL2/SDL_image.h>
#include <stdbool.h>

typedef struct {
  Point position;
  int startX;
  int endX;
  float dy;
  short life;
  int currentSprite;
  bool alive, walking, facingLeft, shooting, visible;

  SDL_Texture *texture;
  void *bullets;
} Humanoid;

void humanoidDefaultConstructor(Humanoid *humanoid);
void humanoidConstructor(Humanoid *humanoid, SDL_Texture *texture,
                         Point position, int startX, int endX, bool facingLeft,
                         int currentSprite, bool alive, bool visible);
void humanoidCopyConstructor(Humanoid *destination, Humanoid *source);
void humanoidDestructor(Humanoid *humanoid);
void jump(Humanoid *humanoid);
void incrementSprite(Humanoid *humanoid);
void stop(Humanoid *humanoid);
void moveLeft(Humanoid *humanoid);
void moveRight(Humanoid *humanoid);
void die(Humanoid *humanoid);
void decreaseLife(Humanoid *humanoid, unsigned int damage);
void shoot(Humanoid *humanoid);
void moveHumanoid(Humanoid *humanoid, Map *map);
void hide(Humanoid *humanoid);
void show(Humanoid *humanoid);
void executeRoutine(Humanoid *humanoid);
bool collidesWithBullet(Humanoid *humanoid, Bullet *bullet);

#endif
