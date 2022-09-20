#include "humanoid.h"
#include "consts.h"
#include "map.h"
#include "vector.h"

void humanoidConstructor(Humanoid *man, Point position, int currentSprite,
                         bool alive, bool visible, bool facingLeft,
                         SDL_Texture *texture) {

  pointCopyConstructor(&man->position, &position);
  man->currentSprite = currentSprite;
  man->alive = alive;
  man->visible = visible;
  man->facingLeft = facingLeft;
  man->texture = texture;
}

void humanoidCopyConstructor(Humanoid *destination, Humanoid *source) {
  humanoidConstructor(destination, source->position, source->currentSprite,
                      source->alive, source->visible, source->facingLeft,
                      source->texture);
}

void humanoidDestructor(Humanoid *man) { SDL_DestroyTexture(man->texture); }

void jump(Humanoid *man) { man->dy = -4 * DELTA_Y; }

void incrementSprite(Humanoid *man) {

  man->currentSprite++;
  man->currentSprite %= 4;
}

void stop(Humanoid *man) {
  man->walking = false;
  man->currentSprite = 4;
}
void moveLeft(Humanoid *man) {

  man->position.x -= DELTA_X;
  man->walking = true;
  man->facingLeft = true;
}

void moveRight(Humanoid *man) {
  man->position.x += DELTA_X;
  man->walking = true;
  man->facingLeft = false;
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

  Bullet bullet;
  Point position;
  if (man->facingLeft) {
    pointConstructor(&position, man->position.x,
                     man->position.y + BULLET_Y_OFFSET);
    bulletConstructor(&bullet, position, -3);
  } else {
    pointConstructor(&position, man->position.x + 30,
                     man->position.y + BULLET_Y_OFFSET);
    bulletConstructor(&bullet, position, 3);
  }
  append(bullets, &bullet);
}

void move(Humanoid *man) {
  man->position.y += man->dy;
  man->dy += DELTA_Y;

  int groundLevel = coordinatesToGroundLevel(man->position.x, man->position.y);

  if (man->position.y > groundLevel) {
    man->position.y = groundLevel;
    man->dy = 0;
  }
}