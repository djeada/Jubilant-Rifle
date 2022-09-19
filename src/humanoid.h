#include "bullet.h"
#include <SDL2/SDL_image.h>
#include <stdbool.h>
typedef struct {
  float x, y, dy;
  short life;
  int currentSprite, walking, facingLeft, shooting, visible;
  bool alive;

  SDL_Texture *sheetTexture;
} Humanoid;

void humanoidConstructor(Humanoid *man, int x, int y, int currentSprite,
                         bool alive, bool visible, bool facingLeft,
                         const char *sheet_path, SDL_Renderer *renderer);
void humanoidDestructor(Humanoid *man);
void jump(Humanoid *man);
void incrementSprite(Humanoid *man);
void stop(Humanoid *man);
void moveLeft(Humanoid *man);
void moveRight(Humanoid *man);
void die(Humanoid *man);
void shoot(Humanoid *man, Bullet **bullets);
