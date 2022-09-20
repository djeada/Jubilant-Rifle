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

void humanoidConstructor(Humanoid *humanoid, Point position, int currentSprite,
                         bool alive, bool visible, bool facingLeft,
                         SDL_Texture *texture);
void humanoidCopyConstructor(Humanoid *destination, Humanoid *source);
void humanoidDestructor(Humanoid *humanoid);
void jump(Humanoid *humanoid);
void incrementSprite(Humanoid *humanoid);
void stop(Humanoid *humanoid);
void moveLeft(Humanoid *humanoid);
void moveRight(Humanoid *humanoid);
void die(Humanoid *humanoid);
void shoot(Humanoid *humanoid, void *bullets);
void moveHumanoid(Humanoid *humanoid);
void hide(Humanoid *humanoid);
void show(Humanoid *humanoid);
bool collidesWithBullet(Humanoid *humanoid, Bullet *bullet);
