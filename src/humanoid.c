#include "humanoid.h"
#include "consts.h"
#include "utils.h"
#include "vector.h"

void humanoidConstructor(Humanoid *man, int x, int y, int currentSprite,
                         bool alive, bool visible, bool facingLeft,
                         SDL_Texture *texture) {
  man->x = x;
  man->y = y;
  man->currentSprite = currentSprite;
  man->alive = alive;
  man->visible = visible;
  man->facingLeft = facingLeft;
  man->texture = texture;
}

void humanoidDestructor(Humanoid *man) { SDL_DestroyTexture(man->texture); }

void jump(Humanoid *man) { man->dy = -4 * DELTA_Y; }

void incrementSprite(Humanoid *man) {

  man->currentSprite++;
  man->currentSprite %= 4;
}

void stop(Humanoid *man) {
  man->walking = 0;
  man->currentSprite = 4;
}
void moveLeft(Humanoid *man) {

  man->x -= DELTA_X;
  man->walking = 1;
  man->facingLeft = 1;
}

void moveRight(Humanoid *man) {
  man->x += DELTA_X;
  man->walking = 1;
  man->facingLeft = 0;
}

void die(Humanoid *man) {
  man->alive = false;
  man->currentSprite = 6;
}

void shoot(Humanoid *man, void *bullets) {
  if (man->currentSprite == 4)
    man->currentSprite = 5;
  else
    man->currentSprite = 4;

  if (man->facingLeft) {
    Bullet bullet;
    bulletConstructor(&bullet, man->x, man->y + BULLET_Y_OFFSET, -3);
    append(bullets, &bullet);
  } else {
    Bullet bullet;
    bulletConstructor(&bullet, man->x + 30, man->y + BULLET_Y_OFFSET, 3);
    append(bullets, &bullet);
  }
}