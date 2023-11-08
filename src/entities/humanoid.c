#include "entities/humanoid.h"
#include "utils/consts.h"

void humanoidConstructor(Humanoid *humanoid, int spriteIndex, bool facingLeft,
                         bool walking, int posX, int posY, int startX, int endX,
                         SDL_Texture *tex, short initialLife, bool alive,
                         bool visible) {
  if (!humanoid)
    return;

  animationStateConstructor(&humanoid->animation, spriteIndex, facingLeft,
                            walking, visible);
  movementStateConstructor(&humanoid->movement, posX, posY, startX, endX);

  humanoid->texture = tex;

  vectorInit(&humanoid->bullets, 10, NULL);

  humanoid->life = initialLife;
  humanoid->isAlive = alive;
}

void humanoidDefaultConstructor(Humanoid *humanoid) {
  if (!humanoid)
    return;

  animationStateConstructor(&humanoid->animation, 0, false, false, true);
  movementStateConstructor(&humanoid->movement, 0, 0, 0, 0);

  humanoid->texture = NULL;
  vectorInit(&humanoid->bullets, 10, NULL);

  humanoid->life = 100;
  humanoid->isAlive = true;
}

void humanoidCopyConstructor(Humanoid *destination, const Humanoid *source) {
  if (!destination || !source)
    return;

  destination->animation = source->animation;
  destination->movement = source->movement;
  destination->texture = source->texture;

  // Deep copy of the bullets Vector.
  destination->bullets = *vectorCopy(&source->bullets);

  destination->life = source->life;
  destination->isAlive = source->isAlive;
}

void humanoidDestructor(void *data) {
  if (!data)
    return;

  Humanoid *humanoid = (Humanoid *)data;

  // Use vectorFree to properly deallocate the Vector and its items.
  vectorFree(&humanoid->bullets);

  // Assuming that texture is managed elsewhere (shared resource), we don't
  // destroy it here.
}

void humanoidDie(Humanoid *humanoid) {
  if (!humanoid)
    return;

  humanoid->isAlive = false;
  animationStateHide(
      &humanoid
           ->animation); // Assuming animationStateHide is defined elsewhere.
}

void humanoidDecreaseLife(Humanoid *humanoid, unsigned int damage) {
  if (!humanoid || !humanoid->isAlive)
    return;

  if ((int)humanoid->life > (int)damage) {
    humanoid->life -= damage;
  } else {
    humanoid->life = 0;
    humanoidDie(humanoid);
  }
}
