#include "humanoid.h"
#include "consts.h"
#include "vector.h"

void humanoidDefaultConstructor(Humanoid *humanoid) {
  humanoid->position.x = 0;
  humanoid->position.y = 0;
  humanoid->startX = 0;
  humanoid->endX = 0;
  humanoid->dy = 0.5;
  humanoid->life = 100;
  humanoid->currentSprite = 0;
  humanoid->alive = false;
  humanoid->walking = false;
  humanoid->facingLeft = false;
  humanoid->shooting = false;
  humanoid->visible = false;
  humanoid->texture = NULL;
  humanoid->bullets = NULL;
}

void humanoidConstructor(Humanoid *humanoid, SDL_Texture *texture,
                         Point position, int startX, int endX,
                         bool facingLeft, int currentSprite, bool alive,
                         bool visible) {

  humanoidDefaultConstructor(humanoid);

  pointCopyConstructor(&(humanoid->position), &position);
  humanoid->startX = startX;
  humanoid->endX = endX;

  humanoid->currentSprite = currentSprite;
  humanoid->alive = alive;
  humanoid->visible = visible;
  humanoid->facingLeft = facingLeft;
  humanoid->texture = texture;

  humanoid->bullets = malloc(sizeof(Vector));
  Vector *bullets = (Vector *)humanoid->bullets;
  vectorConstructor(bullets, MAX_BULLETS, BULLET);
}

void humanoidCopyConstructor(Humanoid *destination, Humanoid *source) {
  humanoidDestructor(destination);
  humanoidConstructor(destination, source->texture, source->position,
                      source->startX, source->endX, source->facingLeft,
                      source->currentSprite, source->alive, source->visible);
}

void humanoidDestructor(Humanoid *humanoid) {
  // SDL_DestroyTexture(humanoid->texture);
  if (humanoid->bullets != NULL) {
    Vector *bullets = (Vector *)humanoid->bullets;
    clear(bullets);
    free(bullets);
    humanoid->bullets = NULL;
  }
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

void decreaseLife(Humanoid *humanoid, unsigned int damage) {
  if (humanoid->life > 0) {
    humanoid->life -= damage;
  }
  if (humanoid->life <= 0) {
    die(humanoid);
  }
}

void shoot(Humanoid *humanoid) {
  humanoid->shooting = true;
  if (humanoid->currentSprite == 4)
    humanoid->currentSprite = 5;
  else
    humanoid->currentSprite = 4;

  Bullet bullet;
  if (humanoid->facingLeft) {

    bulletConstructor(&bullet,
                      createPoint(humanoid->position.x,
                                  humanoid->position.y + BULLET_Y_OFFSET),
                      -3);
  } else {

    bulletConstructor(&bullet,
                      createPoint(humanoid->position.x + BULLET_X_OFFSET,
                                  humanoid->position.y + BULLET_Y_OFFSET),
                      3);
  }
  Vector *bullets = (Vector *)humanoid->bullets;
  append(bullets, &bullet);
}

void moveHumanoid(Humanoid *humanoid, Map *map) {
  humanoid->position.y += humanoid->dy;
  humanoid->dy += DELTA_Y;

  int groundLevel = coordinatesToGroundLevel(map, &(humanoid->position));

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

void executeRoutine(Humanoid *humanoid) {

  if (humanoid->shooting) {
    return;
  }

  // humanoid moves from humanoid->start to humanoid->end
  // then it turns around and moves back

  if (humanoid->facingLeft) {
    moveLeft(humanoid);
    if (humanoid->position.x < humanoid->startX) {
      moveRight(humanoid);
    }
  } else {
    moveRight(humanoid);
    if (humanoid->position.x > humanoid->endX) {
      moveLeft(humanoid);
    }
  }
}
