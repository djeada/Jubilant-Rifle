#include "bullet.h"
#include <SDL2/SDL_image.h>
#include <stdbool.h>

typedef struct {
  Point position;
  float dy;
  short life;
  int currentSprite;
  bool alive, walking, facingLeft, shooting, visible;

  SDL_Texture *texture;
} Humanoid;

void humanoidConstructor(Humanoid *man, Point position, int currentSprite,
                         bool alive, bool visible, bool facingLeft,
                         SDL_Texture *texture);
void humanoidCopyConstructor(Humanoid *destination, Humanoid *source);
void humanoidDestructor(Humanoid *man);
void jump(Humanoid *man);
void incrementSprite(Humanoid *man);
void stop(Humanoid *man);
void moveLeft(Humanoid *man);
void moveRight(Humanoid *man);
void die(Humanoid *man);
void shoot(Humanoid *man, void *bullets);
void move(Humanoid *man);
