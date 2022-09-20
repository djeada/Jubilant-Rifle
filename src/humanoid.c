#include "humanoid.h"
#include "consts.h"
#include "map.h"
#include "vector.h"

void humanoidConstructor(Humanoid *humanoid, Point position, int currentSprite,
                         bool alive, bool visible, bool facingLeft,
                         SDL_Texture *texture) {

  pointCopyConstructor(&humanoid->position, &position);
  humanoid->currentSprite = currentSprite;
  humanoid->alive = alive;
  humanoid->visible = visible;
  humanoid->facingLeft = facingLeft;
  humanoid->texture = texture;
}

void humanoidCopyConstructor(Humanoid *destination, Humanoid *source) {
  humanoidConstructor(destination, source->position, source->currentSprite,
                      source->alive, source->visible, source->facingLeft,
                      source->texture);
}

void humanoidDestructor(Humanoid *humanoid) {
  SDL_DestroyTexture(humanoid->texture);
}

void jump(Humanoid *humanoid) { humanoid->dy = -4 * DELTA_Y; }

void incrementSprite(Humanoid *humanoid) {

  humanoid->currentSprite++;
  humanoid->currentSprite %= 4;
}

void stop(Humanoid *humanoid) {
  humanoid->walking = false;
  humanoid->currentSprite = 4;
}
void moveLeft(Humanoid *humanoid) {

  humanoid->position.x -= DELTA_X;
  humanoid->walking = true;
  humanoid->facingLeft = true;
}

void moveRight(Humanoid *humanoid) {
  humanoid->position.x += DELTA_X;
  humanoid->walking = true;
  humanoid->facingLeft = false;
}

void die(Humanoid *humanoid) {
  humanoid->alive = false;
  humanoid->currentSprite = 6;
}

void shoot(Humanoid *humanoid, void *bullets) {
  if (humanoid->currentSprite == 4)
    humanoid->currentSprite = 5;
  else
    humanoid->currentSprite = 4;

  Bullet bullet;
  Point position;
  if (humanoid->facingLeft) {
    pointConstructor(&position, humanoid->position.x,
                     humanoid->position.y + BULLET_Y_OFFSET);
    bulletConstructor(&bullet, position, -3);
  } else {
    pointConstructor(&position, humanoid->position.x + 30,
                     humanoid->position.y + BULLET_Y_OFFSET);
    bulletConstructor(&bullet, position, 3);
  }
  append(bullets, &bullet);
}

void moveHumanoid(Humanoid *humanoid) {
  humanoid->position.y += humanoid->dy;
  humanoid->dy += DELTA_Y;

  int groundLevel =
      coordinatesToGroundLevel(humanoid->position.x, humanoid->position.y);

  if (humanoid->position.y > groundLevel) {
    humanoid->position.y = groundLevel;
    humanoid->dy = 0;
  }
}

bool collidesWithBullet(Humanoid *humanoid, Bullet *bullet) {

  if (!humanoid->alive) {
    return false;
  }

  return bullet->position.x > humanoid->position.x &&
         bullet->position.x < humanoid->position.x + HUMANOID_WIDTH &&
         bullet->position.y > humanoid->position.y &&
         bullet->position.y < humanoid->position.y + HUMANOID_HEIGHT;
}

void hide(Humanoid *humanoid) { humanoid->visible = false; }

void show(Humanoid *humanoid) { humanoid->visible = true; }